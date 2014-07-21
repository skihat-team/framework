<?php
/**
 * mysql_pdo_engine.inc Mysql的PDO数据访问引擎声明。
 *
 * @copyright   2012-2013 SkiHat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      SkiHat Team
 *
 * @package     skihat.kernels.databases.engines
 * @version     1.0
 *
 * @create      2012-07-17
 * @modify      2013-09-05
 */

# 将PdoEngine引入当前环境。
Skihat::import('kernels.databases.pdo_engine', SKIHAT_PATH_LIBRARY);
Skihat::import('kernels.databases.sql_builder', SKIHAT_PATH_LIBRARY);

/**
 * MySql Pdo数据库引擎，提供MySql的PDO数据库访问服务。
 *
 * @package skihat.kernels.databases.engines
 */
class MysqlPdoEngine extends PdoEngine {
    /**
     * @const string 配置信息，字符集。
     */
    const CONF_CHARSET = 'charset';

    /**
     * @see PdoEngine::_fetchTableSchema
     */
    protected function _fetchTableSchema($table) {
        # 生成查询命令。
        $cmd = 'show columns from ' . $this->sqlBuilder->safeName($table);
        $columns = $this->query($cmd, array(), self::FETCH_STYLE_ASSOC);

        # 处理列的schema信息。
        $schemas = array();
        foreach ($columns as $column) {
            list($type, $size) = $this->_fieldType($column['Type']);
            $schemas[$column['Field']] = array(
                self::SCHEMA_FIELD => $column['Field'],
                self::SCHEMA_TYPE => $type,
                self::SCHEMA_SIZE => $size,
                self::SCHEMA_NULL => $column['Null'] == 'NO' ? false : true,
                self::SCHEMA_DEFAULT => $column['Default'],
                self::SCHEMA_FORMATTER => false,);
        }

        return $schemas;
    }

    /**
     * 根据$dns创建MySql连接对象。
     *
     * @param  array $conf 服务器字符串信息。
     *
     * @return mysqli
     *
     * @throws DatabaseException 如果连接发生错误。
     */
    protected function _createLink(array $conf) {
        # 从配置中获取字符集
        $charset = array_key_pop($conf, self::CONF_CHARSET);
        $link = parent::_createLink($conf);

        if ($charset) {
            $link->exec('SET NAMES ' . $charset);
        }

        return $link;
    }

    /**
     * @see PdoEngine::_createBuilder
     */
    protected function _createBuilder() {
        return new MySqlBuilder();
    }
}

/**
 * MySql Builder MySql的私有化SQL代码生成器。
 *
 * @package skihat.kernels.datbases.engines
 */
class MySqlBuilder extends SqlBuilder {
    /**
     * @see SqlBuilder::safeName
     */
    public function safeName($name) {
        return str_replace('.', '`.`', "`$name`");
    }
}