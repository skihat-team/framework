<?php
/**
 * database_test.inc 提供对数据库组件的测试服务。
 *
 * @copyright   2012-2013 SkiHat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      SkiHat Team
 *
 * @package     skihat.kernels.databases
 * @version     1.0
 *
 * @created     2012-07-13
 * @modify      2013-09-04
 */

# 声明根节点。
require_once(dirname(dirname(dirname(dirname(__FILE__)))) . '/startup.inc');

# 引入被测试类及关联类
Skihat::import('kernels.databases.database', SKIHAT_PATH_LIBRARY);

/**
 * 数据库测试类，提供数据库组件的测试服务。
 *
 * @package skihat.kernels.databases
 */
class DatabaseTest extends PHPUnit_Framework_TestCase {
    /**
     * 测试写入到默认引擎中。
     */
    public function testDefaultEngine() {
        $database = Database::engine();
        $this->assertInstanceOf('DefaultDatabaseEngine', $database);
    }

    /**
     * 测试写入到实例引擎中。
     */
    public function testExampleEngine() {
        $example = Database::engine('example');
        $this->assertInstanceOf('ExampleDatabaseEngine', $example);
    }

    /**
     * 测试读取引擎名称集。
     */
    public function testEngines() {
        $engines = Database::engines();

        $this->assertEquals('default', $engines[0]);
        $this->assertEquals('example', $engines[1]);
    }

    /**
     * 测试初始化方法。
     */
    public function setUp() {
        Skihat::write(Database::CONFIG_NAME, array(
                'default' => '#DefaultDatabaseEngine',
                'example' => '#ExampleDatabaseEngine'
            )
        );

        Database::initialize();
    }
}

/**
 * 默认数据库，提供数据引擎默认器支持。
 *
 * @package skihat.kernels.databases
 */
class DefaultDatabaseEngine implements IDatabaseEngine {
    public function create(array $data, array $syntax) {
    }

    public function createMultiple(array $fields,array $values,array $syntax) {

    }

    public function update($fields, array $syntax) {
    }

    public function delete(array $syntax) {
    }

    public function fetch(array $syntax,$fetchStyle = self::FETCH_STYLE_ASSOC) {
    }

    public function fetchAll(array $syntax, $fetchStyle = self::FETCH_STYLE_ASSOC) {
    }

    public function fetchLastID() {
    }

    public function fetchSchema($table) {
        return array();
    }

    public function quote($param, $type = PDO::PARAM_STR) {

    }

    public function query($cmd, array $args = array(), $options = array()) {

    }

    public function execute($cmd, array $args = array(), $options = array()) {

    }

    protected function _fetchTableSchema($table) {

    }
}

/**
 * 实例数据库，提供数据测试默认服务。
 *
 * @pacakge skihat.kernels.databases
 */
class ExampleDatabaseEngine extends DefaultDatabaseEngine {

}
