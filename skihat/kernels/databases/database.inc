<?php
/**
 * database.inc 数据库接口声明文件。
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

/**
 * 数据库异常信息，当数据库发生错误时将引发本异常。
 *
 * @package skihat.kernels.databases
 */
class DatabaseException extends skihatException {
}

/**
 * 数据库语法异常，当分析数据库语法时发生错误将引发本异常。
 *
 * @package skihat.kernels.databases
 */
class DBSyntaxException extends DatabaseException {
}

/**
 * 数据库普通参数选项，普通参数不需要进行参数化处理。
 *
 * @param skihat .kernels.databases
 */
class NormalParameter {
    /**
     * @var string 参数值。
     */
    protected $_value = '';

    /**
     * 使用$value初始化NormalParameter参数。
     *
     * @param $value
     */
    public function  __construct($value) {
        $this->_value = $value;
    }

    public function __toString() {
        return $this->_value;
    }
}

/**
 * 数据提供接口，提供数据的语法信息。
 *
 * @package skihat.kernels.databases
 */
interface IDataSupport {
    /**
     * @const string 语法常量，指定表字符串。
     */
    const SYNTAX_FROM = 'from';

    /**
     * @const string 语法常量，表名称。
     */
    const SYNTAX_TABLE = 'table';

    /**
     * @const string 语法常量，表别名。
     */
    const SYNTAX_TABLE_ALIAS = 'alias';

    /**
     * @const string 语法常量，数据字段。
     */
    const SYNTAX_FIELDS = 'fields';

    /**
     * @const string 语法常量，where条件。
     */
    const SYNTAX_WHERE = 'where';

    /**
     * @const string 语法常量，排序。
     */
    const SYNTAX_ORDER = 'order';

    /**
     * @const string 语法常量，分页。
     */
    const SYNTAX_PAGE = 'page';

    /**
     * @const string 语法常量 ，模型关系。
     */
    const SYNTAX_LINK = 'links';

    /**
     * @const string 连接关系。
     */
    const SYNTAX_JOIN = 'joins';

    /**
     * @const string 语法常量，连接方式。
     */
    const SYNTAX_JOIN_ON = 'on';

    /**
     * @const string 语法常量，连接类型。
     */
    const SYNTAX_JOIN_TYPE = 'joinType';

    /**
     * @const string 语法常量，分组。
     */
    const SYNTAX_GROUP = 'group';

    /**
     * @const int 检索数据，字段KEY数组。
     */
    const FETCH_STYLE_ASSOC = 2;

    /**
     * @const int 检索数据，数据key数组。
     */
    const FETCH_STYLE_NUM = 3;

    /**
     * @const string 常量选项，获取数据格式为key和index索引。
     */
    const FETCH_STYLE_BOTH = 4;

    /**
     * @const int 检索数据，对象值。
     */
    const FETCH_STYLE_OBJ = 5;

    /**
     * 返回单列的数据值。
     */
    const FETCH_STYLE_COLUMN = 7;

    /**
     * @const int 检索数据，分组值。
     */
    const FETCH_STYLE_GROUP = 65536;

    /**
     * @const int 检索数据，模型数据。
     */
    const FETCH_STYLE_MODEL = 1000;
}

/**
 * 数据库引擎接口，提供数据库的访问服务。
 *
 * @package skihat.kernels.databases
 */
interface IDatabaseEngine extends IDataSupport {
    /**
     * @const string 模式字段,字段名称。
     */
    const SCHEMA_FIELD = 'field';

    /**
     * @const string 模式字段,数据库类型。
     */
    const SCHEMA_TYPE = 'type';

    /**
     * @const string 模式字段，字段大小。
     */
    const SCHEMA_SIZE = 'size';

    /**
     * @const string 模式字段，是否允许为空。
     */
    const SCHEMA_NULL = 'null';

    /**
     * @const string 模式字段，默认值。
     */
    const SCHEMA_DEFAULT = 'default';

    /**
     * @const string 模式字段，格式器。
     */
    const SCHEMA_FORMATTER = 'formatter';

    /**
     * 使用$fields、$syntax和$options创建数据。
     *
     * @param array $fields 创建数据。
     * @param array $syntax 创建语法。
     *
     * @return mixed
     */
    function create(array $fields, array $syntax);

    /**
     * 根据$fields、$syntax和$syntax创建多条记录，并返回响应的行。
     *
     * @param array $fields 字段名称。
     * @param array $values 字段值；
     * @param array $syntax 创建语法；
     *
     * @return mixed
     */
    function createMultiple(array $fields, array $values, array $syntax);

    /**
     * 使用$fields、$syntax和$options更新数据内容。
     *
     * @param mixed $fields  更新字段的值。
     * @param array $syntax  更新语法 。
     * @param mixed $options 更新选项。
     *
     * @return mixed
     */
    function update($fields, array $syntax);

    /**
     * 使用$syntax和$options删除数据。
     *
     * @param array $syntax  删除语法。
     * @param mixed $options 删除选项。
     *
     * @return mixed
     */
    function delete(array $syntax);

    /**
     * 根据$syntax和$options执行数据查询，并返回结果。
     *
     * @param array $syntax  语法结构。
     * @param mixed $options 选项。
     *
     * @return mixed
     */
    function fetchAll(array $syntax, $fetchStyle = self::FETCH_STYLE_ASSOC);

    /**
     * 查询最后插入数据引擎的生成的ID值。
     *
     * @return mixed
     */
    function fetchLastID();

    /**
     * 根据$table查询表格的Schema信息。
     *
     * @param string $table
     *
     * @return array
     */
    function fetchSchema($table);

    /**
     * 使用$cmd和$args执行数据库查询，并返回查询结果。
     *
     * @param string $cmd     SQL查询命令。
     * @param array  $args    SQL查询参数。
     * @param mixed  $options SQL查询参数选项。
     *
     * @return mixed
     */
    function query($cmd, array $args = array(), $fetchStyle = self::FETCH_STYLE_ASSOC);

    /**
     * 使用$cmd、$args和$options 执行数据库命令，并返回响应的行。
     *
     * @param string $cmd     SQL 查询命令。
     * @param array  $args    SQL 查询参数。
     * @param mixed  $options SQL 命令选项。
     *
     * @return int
     */
    function execute($cmd, array $args = array());
}

/**
 * 数据库接口，提供数据引擎访问服务。
 *
 * @package skihat.kernels.databases
 */
abstract class Database {
    /**
     * @const string 数据库调试名称。
     */
    const  DEBUG_NAME = 'kernels/databases';

    /**
     * @const string 数据库配置节点。
     */
    const CONFIG_NAME = 'kernels/databases';

    /**
     * @const string 默认引擎。
     */
    const DEFAULT_ENGINE = 'default';

    /**
     * 工厂初始化方法，执行对象的初始化。
     */
    public static function initialize() {
        self::$_factory = new IocFactory(skihat::read(self::CONFIG_NAME, array()));
    }

    /**
     * 根据$name获取数据库引擎。
     *
     * @param string $name 引擎名字。
     *
     * @return IDatabaseEngine
     */
    public static function engine($name = self::DEFAULT_ENGINE) {
        return self::$_factory->instance($name);
    }

    /**
     * 返回全部配置引擎的名称。
     */
    public static function engines() {
        return self::$_factory->instances();
    }


    /**
     * @var IocFactory 工厂对象。
     */
    protected static $_factory = null;
}

# 执行初始化操作。
Database::initialize();