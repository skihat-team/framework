<?php
/**
 * pdo_engine.inc 提供采用PDO数据库引擎声明服务。
 *
 * @copyright   2012-2013 SkiHat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      SkiHat Team
 *
 * @package     skihat.kernels.databases
 * @version     1.0
 *
 * @create      2012-07-17
 * @modify      2013-09-05
 */

# 引入事务接口。
Skihat::import('kernels.transactions.transaction', SKIHAT_PATH_LIBRARY);

/**
 * PDO 数据库引擎，提供采用PDO方式访问数据库的服务。
 *
 * @package skihat.kernels.databases.engines
 */
abstract class PdoEngine implements IDatabaseEngine, ITransactionObserver {

    /**
     * @const string 配置信息，DSN。
     */
    const CONF_DSN = 'dsn';

    /**
     * @const string 配置信息，用户名。
     */
    const CONF_USER = 'user';

    /**
     * @const string 配置信息，密码。
     */
    const CONF_PASS = 'pass';

    /**
     * @var bool|string PDO访问配置信息。
     */
    public $conf = '';

    /**
     * @var bool|string PDO数据库写入配置信息。
     */
    public $wconf = false;

    /**
     * @var SqlBuilder SQL生成器信息。
     */
    public $sqlBuilder = false;

    /**
     * @var bool 是否支持事务。
     */
    public $transaction = false;

    /**
     * @var bool|PDO Mysql只读访问连接。
     */
    protected $_rlink = false;

    /**
     * @var bool|PDO Mysql只写访问连接。
     */
    protected $_wlink = false;

    /**
     * @var array 表格schema运行时缓存。
     */
    protected static $_tableSchemas = array();

    /**
     * 初始化PDoEngine实例。
     */
    public function __construct() {
        $this->sqlBuilder = $this->_createBuilder();
    }

    /**
     * @see ITransactionObserver::begin
     */
    public function begin() {
        if ($this->transaction) {
            $this->_wlink()->beginTransaction();
        }
    }

    /**
     * @see ITransactionObserver::commit
     */
    public function commit() {
        if ($this->transaction) {
            $this->_wlink()->commit();
        }
    }

    /**
     * @see ITransaction::rollback
     */
    public function rollback() {
        if ($this->transaction) {
            $this->_wlink()->commit();
        }
    }

    /**
     * @see IDatabaseEngine::create
     */
    public function create(array $fields, array $syntax) {
        list($cmd, $args) = $this->sqlBuilder->create($fields, $syntax);
        return $this->execute($cmd, $args);
    }

    /**
     * @see IDatabaseEngine::createMultiple
     */
    public function createMultiple(array $fields, array $values, array $syntax) {
        list($cmd, $args) = $this->sqlBuilder->createMultiple($fields, $values, $syntax);
        return $this->execute($cmd, $args);
    }

    /**
     * @see IDatabaseEngine::update
     */
    public function update($fields, array $syntax) {
        list($cmd, $args) = $this->sqlBuilder->update($fields, $syntax);
        return $this->execute($cmd, $args);
    }

    /**
     * @see IDatabaseEngine::delete
     */
    public function delete(array $syntax) {
        list($cmd, $args) = $this->sqlBuilder->delete($syntax);
        return $this->execute($cmd, $args);
    }

    /**
     * @see IDatabaseEngine::fetchAll
     */
    public function fetchAll(array $syntax, $fetchStyle = self::FETCH_STYLE_ASSOC) {
        list($cmd, $args) = $this->sqlBuilder->fetch($syntax);
        return $this->query($cmd, $args, $fetchStyle);
    }

    /**
     * @see IDatabaseEngine::fetchLastID
     */
    public function fetchLastID() {
        $link = $this->_wlink();
        return $link->lastInsertId();
    }

    /**
     * @see IDatabaseEngine::fetchSchema
     */
    public function fetchSchema($table) {
        # 如果指定table的值，还没有加载的缓存中。
        if (!isset(self::$_tableSchemas[$table])) {
            self::$_tableSchemas[$table] = $this->_fetchTableSchema($table);
        }

        return self::$_tableSchemas[$table];
    }

    /**
     * @see IDatabaseEngine::query
     */
    public function query($cmd, array $args = array(), $fetchStyle = self::FETCH_STYLE_ASSOC) {
        # 如果当前为调试模式，记录执行结果。
        if (SKIHAT_DEBUG) {
            Skihat::debug(Database::DEBUG_NAME, sprintf('PdoEngine->query: cmd - %s,args - %s', $cmd, implode('|', (array)$args)));
        }

        try {
            # 获取只读连接实例。
            $link = $this->_rlink();
            $stmt = $link->prepare($cmd);
            $stmt->execute($args);

            if ($fetchStyle != self::FETCH_STYLE_MODEL) {
                return $stmt->fetchAll($fetchStyle);
            } else {
                return $this->_fetchModel($stmt);
            }
        } catch (PDOException $ex) {
            throw new DatabaseException('PdoEngine:Query error ' . $ex->getMessage(), 0, $ex);
        }
    }

    /**
     * @see IDatabaseEngine::execute
     */
    public function execute($cmd, array $args = array()) {
        # 如果当前为调试模式。
        if (SKIHAT_DEBUG) {
            Skihat::debug(Database::DEBUG_NAME, sprintf('PdoEngine->execute: cmd - %s,args - %s', $cmd, implode('|', (array)$args)));
        }

        try {
            # 获取只写参数，并预处理命令参数。
            $link = $this->_wlink();
            $stmt = $link->prepare($cmd);

            # 执行并返回响应的行。
            $stmt->execute($args);
            return $stmt->rowCount();
        } catch (PDOException $ex) {
            throw new DatabaseException('PdoEngine:execute error ' . $ex->getMessage(), 0, $ex);
        }
    }

    /**
     * 根据$stmt返回模型声明的数组结构。
     *
     * @param PDOStatement $stmt 模型声明
     *
     * @return array
     */
    protected function _fetchModel($stmt) {
        $meta = array();
        $columnCount = $stmt->columnCount();
        for ($i = 0; $i < $columnCount; $i++) {
            $meta[$i] = $stmt->getColumnMeta($i);
        }

        $models = array();
        $rows = $stmt->fetchAll(PDO::FETCH_NUM);
        foreach ($rows as $row) {
            $model = array();
            for ($i = 0; $i < $columnCount; ++$i) {
                $model[$meta[$i]['table']][$meta[$i]['name']] = $row[$i];
            }

            array_push($models, $model);
        }

        return $models;
    }

    /**
     * 根据$table查询表格的schema信息并返回。
     *
     * @param string $table 表格名称。
     *
     * @return array
     */
    protected abstract function _fetchTableSchema($table);

    /**
     * 根据数据列表类型，分隔出类型和大小。
     *
     * @param string $type 数据列。
     *
     * @return array
     */
    protected function _fieldType($type) {
        # int(1) number(1,10)
        if ($pos = strpos($type, '(')) {
            $dbtype = substr($type, 0, $pos);
            $dbsize = intval(substr($type, $pos + 1, -1));
        } else {
            $dbtype = $type;
            $dbsize = false;
        }

        return array($dbtype, $dbsize);
    }

    /**
     * 获取只读PDO连接对象。
     *
     * @return PDO
     */
    protected function _rlink() {
        # 如果只读连接还没有创建。
        if (!$this->_rlink) {
            $this->_rlink = $this->_createLinkForConfig($this->conf);
        }

        return $this->_rlink;
    }

    /**
     * 获取只写PDO连接对象。
     *
     * @return PDO
     */
    protected function _wlink() {
        # 如果只读连接还没有创建。
        if (!$this->_wlink) {
            $this->_wlink = $this->wconf ? $this->_createLinkForConfig($this->wconf) : $this->_rlink();

            # 如果当前数据库支持事务操作。
            if ($this->transaction) {
                Transaction::register($this);
            }
        }

        return $this->_wlink;
    }

    /**
     * 根据配置信息创建数据库连接。
     *
     * @param string $conf 配置值。
     *
     * @return PDO
     */
    protected function _createLinkForConfig($conf) {
        return $this->_createLink($this->_resolveConfig($conf));
    }

    /**
     * 根据$dsn配置信息创建PDO对象并返回。
     *
     * @param array $conf 数据库访问配置。
     *
     * @return PDO
     *
     * @throws ConfigException 如果配置发生错误则引发本异常。
     */
    protected function _createLink(array $conf) {
        # 获取dsn配置信息。
        if (!($dsn = array_key_pop($conf, self::CONF_DSN))) {
            throw new ConfigException('PdoEngine:conf not support "dsn".');
        }
        $user = array_key_pop($conf, self::CONF_USER, false);
        $pass = array_key_pop($conf, self::CONF_PASS, false);

        # 创建PDO实例，并设置错误处理方式为异常处理。
        $link = new PDO($dsn, $user, $pass, $conf);
        $link->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        return $link;
    }

    /**
     * 通过配置参数，获取到设置的数组值。
     *
     * @param string $conf 配置参数。
     *
     * @return array
     */
    protected function _resolveConfig($conf) {
        # 如果没有进行配设置，直接返回空数组。
        if (!$conf) return array();

        $args = explode('&', $conf);
        $conf = array(self::CONF_DSN => array_key_pop($args, 0, false));

        # 从参数中分解出名称和值。
        foreach ($args as $cf) {
            list($name, $val) = explode('=', $cf);
            $conf[$name] = $val;
        }

        return $conf;
    }

    /**
     * 创建SqlBuilder实例。
     *
     * @return SqlBuilder
     */
    protected abstract function _createBuilder();
}