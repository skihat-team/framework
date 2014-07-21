<?php
/**
 * model_meta.inc 提供模型元数据声明。
 *
 * @copyright   2012-2013 SkiHat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      SkiHat Team
 *
 * @package     skihat.models
 * @version     1.0
 *
 * @created     2012-07-23
 * @modify      2013-09-03
 */

/**
 * 模型元数据提供者对象，提供模型所需要元数据描述。
 *
 * @package skihat.models
 */
interface IModelMetaSupport extends IDataSupport {
    /**
     * @const string 模型元数据配置方法名称。
     */
    const CONFIG_METHOD = 'config';

    /**
     * @const string 默认id值。
     */
    const DEFAULT_PRIMARY_KEY = 'id';

    /**
     * @const int 默认主键自增选项。
     */
    const DEFAULT_PRIMARY_INCREMENT = true;

    /**
     * @const string 元数据，数据库。
     */
    const META_DATABASE = 'database';

    /**
     * @const string 元数据，表名称。
     */
    const META_TABLE = self::SYNTAX_TABLE;

    /**
     * @const string 元数据，表别名。
     */
    const META_TABLE_ALIAS = self::SYNTAX_TABLE_ALIAS;

    /**
     * @const string 元数据，主角名称。
     */
    const META_PRIMARY_KEY = 'primaryKey';

    /**
     * @const string 元数据，主键自动生成。
     */
    const META_PRIMARY_INCREMENT = 'primaryIncrement';

    /**
     * @const string 元数据，默认值。
     */
    const META_DEFAULTS = 'default';

    /**
     * @const string 元数据，只读属性。
     */
    const META_READONLY = 'readonly';

    /**
     * @const string 元数据，虚拟字段。
     */
    const META_VIRTUAL = 'virtual';

    /**
     * @const string 元数据，对象关系。
     */
    const META_LINKS = 'links';

    /**
     * @const string 元数据，关系类。
     */
    const META_LINKS_CLASS = 'class';

    /**
     * @const string 元数据，关系类型。
     */
    const META_LINKS_TYPE = 'type';

    /**
     * @const string 元数据，关系连接条件。
     */
    const META_LINKS_ON = self::SYNTAX_JOIN_ON;

    /**
     * @const string 元数据，关系外键。
     */
    const META_LINKS_FOREIGN = 'foreign';

    /**
     * @const string 元数据，连接关系表。
     */
    const META_LINKS_TABLE = 'linkTable';

    /**
     * @const string 元数据，外键关系外键。
     */
    const META_LINKS_TABLE_FOREIGN = 'linkForeign';

    /**
     * @const string 模型验证规则。
     */
    const META_RULE = 'rules';

    /**
     * @const string 元数据，验证器错误。
     */
    const META_RULE_MESSAGE = 'ruleMessages';

    /**
     * @const string 对象关系，一对一。
     */
    const HAS_ONE = 'hasOne';

    /**
     * @const string 对象关系，一对多。
     */
    const HAS_MANY = 'hasMany';

    /**
     * @const string 对象关系，多对一。
     */
    const BELONGS_TO = 'belongsTo';

    /**
     * @const string 对象关系，多对多。
     */
    const HAS_AND_BELONGS = 'hasAndBelongs';

    /**
     * 返回当前模型的元素描述信息声明。
     *
     * @return array
     */
    static function config();
}

/**
 * 模型描述，提供模型的相关配置声明。
 *
 * @package skihat.models
 */
class ModelMeta {
    /**
     * @var string 模型数据库名称。
     */
    public $database = Database::DEFAULT_ENGINE;

    /**
     * @var string 模型名称。
     */
    public $modelName = '';

    /**
     * @var string 模型对应表名称。
     */
    public $table = '';

    /**
     * @var string 模型表别名。
     */
    public $tableAlias = '';

    /**
     * @var string 主键名称。
     */
    public $primaryKey = IModelMetaSupport::DEFAULT_PRIMARY_KEY;

    /**
     * @var bool 自动主键声明。
     */
    public $primaryIncrement = IModelMetaSupport::DEFAULT_PRIMARY_INCREMENT;

    /**
     * @var array 数据库表描述。
     */
    public $schemas = array();

    /**
     * @var array 数据关系描述。
     */
    public $links = array();

    /**
     * @var array 数据默认值。
     */
    public $defaults = array();

    /**
     * @var array 数据只读属性。
     */
    public $readonly = array();

    /**
     * @var array 虚拟字段。
     */
    public $virtual = array();

    /**
     * @var array 验证器声明。
     */
    public $validators = array();

    /**
     * @var array 模型验证器消息。
     */
    public $validatorMessages = array();

    /**
     * 根据$modelName初始化$modelName实例。
     *
     * @param string $modelName 模型名称。
     */
    protected function __construct($modelName) {
        # 从模型中读取meta值。
        $this->_initialize($modelName);
        $this->_initializeProps();
        $this->_initializeLinks();
        $this->_initializeSchemas();
        $this->_initializeRules();
    }

    /**
     * 根据$modelName返回模型元数据信息。
     *
     * @param string $modelName 模型名称。
     *
     * @return ModelMeta
     */
    public static function & instance($modelName) {
        # 如果指定的模型描述信息还没有加载进来。
        if (!isset(self::$_metas[$modelName])) {
            if (enable_cached()) {
                $cacheName = 'model-meta-' . strtolower($modelName);
                # 如果从缓存中读取值成功。
                if ($modelMeta = Cache::read($cacheName, Cache::RUNTIME_ENGINE)) {
                    self::$_metas[$modelName] = $modelMeta;
                } else {
                    self::$_metas[$modelName] = new ModelMeta($modelName);
                    Cache::write($cacheName, self::$_metas[$modelName], Cache::RUNTIME_ENGINE);
                }
            } else {
                self::$_metas[$modelName] = new ModelMeta($modelName);
            }
        }

        return self::$_metas[$modelName];
    }

    /**
     * 根据$modelName执行实例的基本初始工作。
     *
     * @param string $modelName 模型名称。
     */
    protected function _initialize($modelName) {
        $this->modelName = $modelName;
        $this->_config = (array)call_user_func(array($modelName, IModelMetaSupport::CONFIG_METHOD));
    }

    /**
     * 根据$meta初始化ModelMeta对象实例。
     */
    protected function _initializeProps() {
        # 获取配置信息，并设置相关值。
        if (isset($this->_config[IModelMetaSupport::META_DATABASE])) {
            $this->database = $this->_config[IModelMetaSupport::META_DATABASE];
        }

        # 设置当前表名称和主键属性值。
        list($this->primaryKey, $this->primaryIncrement) = $this->_primary($this->_config);
        list($this->table, $this->tableAlias) = $this->_table($this->modelName, $this->_config);

        # 默认值。
        if (isset($this->_config[IModelMetaSupport::META_DEFAULTS])) {
            $this->defaults = (array)$this->_config[IModelMetaSupport::META_DEFAULTS];
        }

        # 只读属性
        if (isset($this->_config[IModelMetaSupport::META_READONLY])) {
            $this->readonly = (array)$this->_config[IModelMetaSupport::META_READONLY];
        }

        # 虚拟字段属性。
        if (isset($this->_config[IModelMetaSupport::META_VIRTUAL])) {
            $this->virtual = (array)$this->_config[IModelMetaSupport::META_VIRTUAL];
        }
    }

    /**
     * 根据$modelName和$config获取数据表的配置信息。
     *
     * @param string $modelName 模型名称。
     * @param array  $config    配置
     *
     * @return array
     */
    protected function _table($modelName, array & $config) {
        # 从配置信息中读取配置选项。
        if (isset($config[IModelMetaSupport::META_TABLE])) {
            $table = $config[IModelMetaSupport::META_TABLE];
        } else {
            $table = camel_lower($modelName);
        }

        if (isset($config[IModelMetaSupport::META_TABLE_ALIAS])) {
            $tableAlias = $config[IModelMetaSupport::META_TABLE_ALIAS];
        } else {
            $tableAlias = pascal($modelName);
        }

        return array($table, $tableAlias);
    }

    /**
     * 根据$config获取表的主键描述。
     *
     * @param array $config 配置信息。
     *
     * @return array
     */
    protected function _primary(array & $config) {
        if (isset($config[IModelMetaSupport::META_PRIMARY_KEY])) {
            $key = $config[IModelMetaSupport::META_PRIMARY_KEY];
        } else {
            $key = IModelMetaSupport::DEFAULT_PRIMARY_KEY;
        }

        if (isset($config[IModelMetaSupport::META_PRIMARY_INCREMENT])) {
            $increment = $config[IModelMetaSupport::META_PRIMARY_INCREMENT];
        } else {
            $increment = true;
        }

        return array($key, $increment);
    }

    /**
     * 初始化schemas值。
     */
    protected function _initializeSchemas() {
        $db = Database::engine($this->database);
        $this->schemas = $db->fetchSchema($this->table);
    }

    /**
     * 初始化连接关系。
     */
    protected function _initializeLinks() {
        # 如果配置中存在连接设置。
        if (isset($this->_config[IModelMetaSupport::META_LINKS])) {
            foreach ($this->_config[IModelMetaSupport::META_LINKS] as $name => $cfgLink) {
                $this->links[$name] = $this->_linkSchema($cfgLink);
            }
        }
    }

    /**
     * 根据$cfg获取连接关系的Schema数据库配置值。
     *
     * @param array $cfg
     *
     * @return array
     *
     * @throws ModelException 如果关系中没有指定模型信息，将引发异常。
     */
    protected function _linkSchema(array $cfg) {
        # 如果没有指定class信息，则引发异常。
        if (empty($cfg[IModelMetaSupport::META_LINKS_CLASS])) {
            throw new ModelException('ModelMeta:Not support link class.');
        }

        # 引用并获取模型名称。
        $linkModel = Skihat::using($cfg[IModelMetaSupport::META_LINKS_CLASS]);
        $linkConfig = (array)call_user_func(array($linkModel, IModelMetaSupport::CONFIG_METHOD));

        # 设置默认连接类型。
        list($cfg[IModelMetaSupport::META_TABLE], $linkTableAlias) = $this->_table($linkModel, $linkConfig);
        if (empty($cfg[IModelMetaSupport::META_TABLE_ALIAS])) {
            $cfg[IModelMetaSupport::META_TABLE_ALIAS] = $linkTableAlias;
        }

        # 设置默认连接类型。
        if (empty($cfg[IModelMetaSupport::META_LINKS_TYPE])) {
            $cfg[IModelMetaSupport::META_LINKS_TYPE] = IModelMetaSupport::HAS_ONE;
        }

        # 设置默认连接外键和ON值。
        list($linkPrimaryKey, $linkPrimaryIncrement) = $this->_primary($linkConfig);
        if (empty($cfg[IModelMetaSupport::META_LINKS_FOREIGN])) {
            $cfg[IModelMetaSupport::META_LINKS_FOREIGN] =
                $this->_defaultLinkForeign($cfg[IModelMetaSupport::META_LINKS_TYPE], $cfg[IModelMetaSupport::META_TABLE]);
        }

        if (empty($cfg[IModelMetaSupport::META_LINKS_ON])) {
            $cfg[IModelMetaSupport::META_LINKS_ON] =
                $this->_defaultLinkOn($cfg[IModelMetaSupport::META_LINKS_TYPE], $cfg[IModelMetaSupport::META_TABLE_ALIAS], $linkPrimaryKey, $cfg[IModelMetaSupport::META_LINKS_FOREIGN]);
        }

        # 如果关系是多对多关系，进行额外的处理。
        if ($cfg[IModelMetaSupport::META_LINKS_TYPE] == IModelMetaSupport::HAS_AND_BELONGS) {
            # 如果多对多关系没有指定额外的表，则生成表的内容。
            if (empty($cfg[IModelMetaSupport::META_LINKS_TABLE])) {
                $cfg = $this->_defaultLinkTable($cfg);
            }

            # 如果没有指定中间表的关联外键值，则增加内容。
            if (empty($cfg[IModelMetaSupport::META_LINKS_TABLE_FOREIGN])) {
                $cfg[IModelMetaSupport::META_LINKS_TABLE_FOREIGN] =
                    $this->_defaultLinkForeign(IModelMetaSupport::BELONGS_TO, $cfg[IModelMetaSupport::META_TABLE]);
            }
        }

        return $cfg;
    }

    /**
     * 根据$type和$modelName获取默认的外键编号。
     *
     * @param string  $type      连接类型。
     * @param  string $linkTable 关系表名称。
     *
     * @return string
     */
    protected function _defaultLinkForeign($type, $linkTable) {
        switch ($type) {
            case IModelMetaSupport::BELONGS_TO:
                return $linkTable . '_id';
            default:
                return $this->table . '_id';
        }
    }

    /**
     * 根据$type、$modelName和$keyName生成自动ON处理。
     *
     * @param string $type        连接类型。
     * @param string $linkAlias   模型名称。
     * @param string $linkKeyName 主键名称。
     * @param string $foreign     外键值。
     *
     * @return string
     */
    protected function _defaultLinkOn($type, $linkAlias, $linkKeyName, $foreign) {
        switch ($type) {
            case IModelMetaSupport::BELONGS_TO:
                return sprintf('%s.%s = %s.%s', $this->tableAlias, $foreign, $linkAlias, $linkKeyName);
            default:
                return sprintf('%s.%s = %s.%s', $linkAlias, $foreign, $this->tableAlias, $this->primaryKey);
        }
    }

    /**
     * 根据$linkCfg生成多对多关系的中间表。
     *
     * @param array $linkCfg 连接配置信息。
     *
     * @return array
     */
    protected function _defaultLinkTable(array & $linkCfg) {
        $tables = array($linkCfg[IModelMetaSupport::META_TABLE], $this->table);
        sort($tables, SORT_STRING);

        $linkCfg[IModelMetaSupport::META_LINKS_TABLE] = camel_lower(implode('_', $tables));

        return $linkCfg;
    }

    /**
     * 根据$meta初始化验证器信息。
     */
    protected function _initializeRules() {
        # 判断理否包含验证器，如果有则设置验证器。
        if (isset($this->_config[IModelMetaSupport::META_RULE])) {
            $this->validators = (array)$this->_config[IModelMetaSupport::META_RULE];
        }

        if (isset($this->_config[IModelMetaSupport::META_RULE_MESSAGE])) {
            $this->validatorMessages = (array)$this->_config[IModelMetaSupport::META_RULE_MESSAGE];
        }
    }

    /**
     * @var array 模型原始meta值。
     */
    protected $_config = array();

    /**
     * @var array 全部meta信息。
     */
    protected static $_metas = array();
}