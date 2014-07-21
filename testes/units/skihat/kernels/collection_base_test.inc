<?php
/**
 * collection_base_test.inc 提供CollectionBase类的单元测试服务。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels
 * @version   1.0
 *
 * @created   2012-06-12
 * @modified  2013-08-02
 */

# 引用测试启动文件。
require_once(dirname(dirname(dirname(__FILE__))) . '/startup.inc');

/**
 * 基础集合类单元测试类，提供对CollectionBase类的单元测试服务。
 *
 * @package skihat.kernels
 */
class CollectionBaseTest extends PHPUnit_Framework_TestCase {
    /**
     * 测试索引值是否存在。
     */
    public function testOffsetExists() {
        $collection = new DefaultCollection(array('name' => 'skihat', 'age' => 2));

        $this->assertTrue(isset($collection['name']));
        $this->assertTrue(isset($collection['age']));
        $this->assertFalse(isset($collection['not-exists']));
    }

    /**
     * 测试索引值，GET读取操作。
     */
    public function testOffsetGet() {
        $collection = new DefaultCollection(array('name' => 'skihat', 'age' => 2));

        $this->assertEquals('skihat', $collection['name']);
        $this->assertEquals(2, $collection['age']);
    }

    /**
     * 测试读取值，索引不存在。
     */
    public function testOffsetGetForNotExists() {
        $collection = new DefaultCollection(array('name' => 'skihat'));

        $this->assertNull($collection['not-exists']);
        $this->assertNull($collection['age']);
    }

    /**
     * 测试写入索引值。
     */
    public function testOffsetSet() {
        $collection = new DefaultCollection();

        $collection['name'] = 'skihat';
        $collection['age'] = 2;

        $this->assertEquals('skihat', $collection['name']);
        $this->assertEquals(2, $collection['age']);
    }

    /**
     * 测试写入索引值，覆盖已有的值。
     */
    public function testOffsetSetForOverride() {
        $collection = new DefaultCollection(array('name' => 'skihat'));
        $collection['name'] = 'world';

        $this->assertEquals('world', $collection['name']);
    }

    /**
     * 测试删除值。
     */
    public function testOffsetUnset() {
        $collection = new DefaultCollection(array('name' => 'skihat', 'age' => 2));

        unset($collection['name']);
        $this->assertNull($collection['name']);

        unset($collection['age']);
        $this->assertNull($collection['age']);
    }
}

/**
 * 默认集合类，提供对CollectionBase类单元测试的服务支持。
 *
 * @package skihat.kernels
 */
class DefaultCollection extends CollectionBase {
    public function __construct(array $vars = array()) {
        $this->_vars = $vars;
    }
}