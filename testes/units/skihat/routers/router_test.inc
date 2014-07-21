<?php
/**
 * router_test.inc 路由单元测试，提供路由单元测试服务。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.routers
 * @version   1.0
 *
 * @created   2012-07-23
 * @modified  2013-08-28
 */

# 引用测试启动文件。
require_once(dirname(dirname(dirname(__FILE__))) . '/startup.inc');

# 引用被测试类。
Skihat::import('routers.router', SKIHAT_PATH_LIBRARY);

/**
 * 路由测试类，提供路由组合接口的单元测试服务。
 *
 * @package skihat.routers
 */
class RouterTest extends PHPUnit_Framework_TestCase {

    /**
     * 测试增加路由规则。
     */
    public function testRule() {
        $router = DefaultRouter::domain('www.example.com');
        $router->rule('#NormalRouterRule');
        $router->rule('#RegexRouterRule');

        $rules = DefaultRouter::rules();
        $links = DefaultRouter::linkRules();

        $this->assertEquals(2, count($rules));
        $this->assertEquals(1, count($links));

        $this->assertEquals('#NormalRouterRule', $rules[0]['rule']);
        $this->assertEquals('#RegexRouterRule', $rules[1]['rule']);

        $this->assertEquals(1, count($links));
        $this->assertTrue(is_array($links['www.example.com']));
        $this->assertEquals(2, count($links['www.example.com']));

        $this->assertEquals('#NormalRouterRule', $links['www.example.com'][0]['rule']);
        $this->assertEquals('#RegexRouterRule', $links['www.example.com'][1]['rule']);
    }

    /**
     * 测试增加路由规则，使用字符串名称。
     */
    public function testRuleForStringName() {
        $router = DefaultRouter::domain('www.example.com');
        $router->rule('#NormalRouterRule', 'example');
        $router->rule('#RegexRouterRule');

        $links = DefaultRouter::linkRules();

        $this->assertEquals(2, count($links));
        $this->assertTrue(is_array($links['example']));
        $this->assertEquals(1, count($links['example']));

        $this->assertEquals('#NormalRouterRule', $links['example'][0]['rule']);
    }


    /**
     * 测试增加路由规则，使用数组名称。
     */
    public function testRuleForArrayName() {
        $router = DefaultRouter::domain('www.example.com');
        $router->rule('#NormalRouterRule', array('example', 'admins'));
        $router->rule('#RegexRouterRule');

        $links = DefaultRouter::linkRules();

        $this->assertEquals(3, count($links));
        $this->assertTrue(is_array($links['example']));
        $this->assertEquals(1, count($links['example']));

        $this->assertTrue(is_array($links['admins']));
        $this->assertEquals(1, count($links['admins']));

        $this->assertEquals('#NormalRouterRule', $links['example'][0]['rule']);
        $this->assertEquals('#NormalRouterRule', $links['admins'][0]['rule']);
    }

    /**
     * 测试规则匹配。
     */
    public function testRuleForMatch() {
        DefaultRouter::domain('www.example.com')->rule(array(SKIHAT_IOC_CLASS => '#NormalRouterRule'));
        $params = DefaultRouter::match(array('controller' => 'goods', 'action' => 'show'));

        $this->assertTrue(is_array($params));
        $this->assertEquals('goods', $params['controller']);
        $this->assertEquals('show', $params['action']);
    }

    /**
     * 测试规则匹配，匹配失败。
     */
    public function testRuleForMatchEmptyFail() {
        DefaultRouter::domain('www.example.com');
        $this->assertFalse(DefaultRouter::match(array('controller' => 'goods', 'action' => 'show')));
    }

    /**
     * 测试参数匹配，验证失败。
     */
    public function testMatchForRuleFail() {
        DefaultRouter::domain('www.example.com')
            ->rule(array(SKIHAT_IOC_CLASS => '#NormalRouterRule', 'controller' => array(NULL, 'members')));

        $this->assertFalse(DefaultRouter::match(array('controller' => 'goods', 'action' => 'show')));
    }

    /**
     * 测试URL生成。
     */
    public function testUrl() {
        DefaultRouter::domain('www.example.com')->rule(array(SKIHAT_IOC_CLASS => '#NormalRouterRule'));
        $this->assertEquals('/?controller=goods&action=show', DefaultRouter::url(array('controller' => 'goods', 'action' => 'show')));
    }

    /**
     * 测试URL生成，指定规则名字。
     */
    public function testUrlForName() {
        DefaultRouter::domain('shop.example.com')
            ->style(array('domain' => 'http://shop.example.com', SKIHAT_IOC_CLASS => '#NormalRouterStyle'))
            ->rule(array(SKIHAT_IOC_CLASS => '#NormalRouterRule'), 'shop');

        $this->assertEquals('http://shop.example.com/?controller=goods&action=show',
            DefaultRouter::url(array('controller' => 'goods', 'action' => 'show', 'ruleName' => 'shop')));
    }

    /**
     * 测试URL生成，指定基础参数。
     */
    public function testUrlForDefaultParams() {
        # 初始化路由
        DefaultRouter::domain('www.example.com')->rule(array(SKIHAT_IOC_CLASS => '#NormalRouterRule'));
        DefaultRouter::defaultParams(array('package' => 'goods'), array('package' => 'goods', 'id' => 12));

        $this->assertEquals('/?package=goods&controller=goods&action=show',
            DefaultRouter::url(array('controller' => 'goods', 'action' => 'show')));
        $this->assertEquals('/?package=goods&id=12&controller=goods&action=show',
            DefaultRouter::url(array('controller' => 'goods', 'action' => 'show', 'fullUrl' => TRUE)));
    }

    /**
     * @see PHPUnit_Framework_TestCase
     */
    public function setUp() {
        DefaultRouter::reset();
    }
}

/**
 * 默认路由器，提供路由测试服务支持。
 *
 * @package skihat.routers
 */
class DefaultRouter extends Router {
    public static function reset() {
        self::$_rules = array();
        self::$_routers = array();
        self::$_linkRules = array();
    }

    public static function rules() {
        return self::$_rules;
    }

    public static function linkRules() {
        return self::$_linkRules;
    }
}