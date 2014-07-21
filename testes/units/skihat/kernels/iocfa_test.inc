<?php
/**
 * iocfa_test.inc 提供IocFactory单元测试服务。
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

require_once(dirname(dirname(dirname(__FILE__))) . '/startup.inc');

/**
 * IocFactory 单元测试类，提供IocFactory的单元测试服务。
 *
 * @package skihat.kernels
 */
class IocFactoryTest extends PHPUnit_Framework_TestCase {

    /**
     * 测试返回全部实例名称。
     */
    public function testInstances() {
        $factory = new IocFactory(array('default' => '#MySqlEngine', 'light' => 'Oracle'));
        $instances = $factory->instances();

        $this->assertEquals('default', $instances[0]);
        $this->assertEquals('light', $instances[1]);
    }

    /**
     * 测试创建实例。
     */
    public function testInstance() {
        $factory = new IocFactory(array('default' => array(SKIHAT_IOC_CLASS => '#IocMock', 'field' => 'skihat')));
        $obj = $factory->instance('default');

        $this->assertInstanceOf('IocMock', $obj);
        $this->assertEquals('skihat', $obj->field);
    }

    /**
     * 测试创建实例，重复创建。
     */
    public function testInstanceForRepeat() {
        $factory = new IocFactory(array('default' => array(SKIHAT_IOC_CLASS => '#IocMock', 'field' => 'skihat')));
        $obj = $factory->instance('default');
        $obj->field = 'world';

        $obj1 = $factory->instance('default');
        $this->assertEquals('world', $obj1->field);
    }

    /**
     * 测试创建实例，没有提供的实例。
     *
     * @expectedException ConfigException
     */
    public function testInstanceForNotSupport() {
        $factory = new IocFactory(array());
        $factory->instance('default');
    }

}

/**
 * Ioc 工厂实例模拟对象。
 *
 * @package skihat.kernels
 */
class IocMock {
    public $filed = false;
}