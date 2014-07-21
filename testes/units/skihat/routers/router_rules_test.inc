<?php
/**
 * router_rules_test.inc 路由规则样式文件，提供路由规则单元测试声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.routers
 * @version   1.0
 *
 * @created   2012-07-23
 * @modified  2013-08-25
 */

# 引用测试启动文件。
require_once(dirname(dirname(dirname(__FILE__))) . '/startup.inc');
# 引用被测试类。
Skihat::import('routers.router', SKIHAT_PATH_LIBRARY);

/**
 * 路由规则测试类，提供路由规则的单元测试服务。
 *
 * @package skihat.routers
 */
class RouterRuleTest extends PHPUnit_Framework_TestCase {
    /**
     * 测试普通规则转换。
     */
    public function testNormalRuleParse() {
        $rule = $this->_createNormalRule();
        $params = $rule->parse(array('action' => 'edit', 'controller' => 'users'));

        $this->assertEquals(2, count($params));
        $this->assertEquals('users', $params['controller']);
        $this->assertEquals('edit', $params['action']);
    }

    /**
     * 测试普通路规则反转。
     */
    public function testNormalRuleReverse() {
        $rule = $this->_createNormalRule();
        $url = $rule->reverse(array('action' => 'edit', 'controller' => 'users'));

        $this->assertEquals(2, count($url));
        $this->assertEquals('users', $url['controller']);
        $this->assertEquals('edit', $url['action']);
    }

    /**
     * 测试普通路由规则，指定默认值。
     */
    public function testNormalRuleParseForDefaults() {
        $rule = $this->_createNormalRule(array(
            'controller' => 'index', 'action' => array('index')
        ));
        $url = $rule->parse(array('action' => 'edit', 'name' => 'skihat'));

        $this->assertEquals(3, count($url));
        $this->assertEquals('index', $url['controller']);
        $this->assertEquals('edit', $url['action']);
        $this->assertEquals('skihat', $url['name']);
    }

    /**
     * 测试普通路由规则反转，指定默认值。
     */
    public function testNormalRuleReverseForDefaults() {
        $rule = $this->_createNormalRule(array(
            'controller' => 'index', 'action' => 'index'
        ));
        $url = $rule->reverse(array('action' => 'index', 'controller' => 'users'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users', $url['controller']);
    }

    /**
     * 测试普通路由规则，指定验证器。
     */
    public function testNormalRuleParseForValidator() {
        $rule = $this->_createNormalRule(array(
            'name' => array(false, '[a-z]+')
        ));

        $this->assertFalse($rule->parse(array('name' => 'abc123')));
        $this->assertNotEmpty($rule->parse(array('name' => 'skihat')));
    }

    /**
     * 测试普通路由规则反转，指定验证器。
     */
    public function testNormalRuleReverseForValidator() {
        $rule = $this->_createNormalRule(array(
            'name' => array(false, '[a-z]+')
        ));

        $this->assertFalse($rule->reverse(array('name' => 'abc123')));
        $this->assertNotEmpty($rule->reverse(array('name' => 'skihat')));
    }

    /**
     * 测试普通路由规则转换，默认验证器。
     */
    public function testNormalRuleParseForDefaultValidator() {
        $rule = $this->_createNormalRule();

        $this->assertNotEmpty($rule->parse(array(SKIHAT_PARAM_MODULE => 'a1-_.')));
        $this->assertNotEmpty($rule->parse(array(SKIHAT_PARAM_PACKAGE => 'a1-_.')));
        $this->assertNotEmpty($rule->parse(array(SKIHAT_PARAM_CONTROLLER => 'users_')));
        $this->assertNotEmpty($rule->parse(array(SKIHAT_PARAM_ACTION => 'edit_')));
        $this->assertNotEmpty($rule->parse(array(SKIHAT_PARAM_ID => '123')));
    }

    /**
     * 测试普通路由规则反转，使用默认验证器。
     */
    public function testNormalRuleReverseForDefaultValidator() {
        $rule = $this->_createNormalRule();

        $this->assertNotEmpty($rule->reverse(array(SKIHAT_PARAM_MODULE => 'a1-_.')));
        $this->assertNotEmpty($rule->reverse(array(SKIHAT_PARAM_PACKAGE => 'a1-_.')));
        $this->assertNotEmpty($rule->reverse(array(SKIHAT_PARAM_CONTROLLER => 'users_')));
        $this->assertNotEmpty($rule->reverse(array(SKIHAT_PARAM_ACTION => 'edit_')));
        $this->assertNotEmpty($rule->reverse(array(SKIHAT_PARAM_ID => '123')));
    }

    /**
     * 测试普通路由规则转换，使用默认验证器验证失败。
     */
    public function testNormalRuleParseForDefaultValidatorFail() {
        $rule = $this->_createNormalRule();

        $this->assertFalse($rule->parse(array(SKIHAT_PARAM_CONTROLLER => 'users123')));
        $this->assertFalse($rule->parse(array(SKIHAT_PARAM_ACTION => 'edit123')));
        $this->assertFalse($rule->parse(array(SKIHAT_PARAM_PACKAGE => 'package<123')));
        $this->assertFalse($rule->parse(array(SKIHAT_PARAM_MODULE => 'module<123')));
        $this->assertFalse($rule->parse(array(SKIHAT_PARAM_ID => 'abc')));
    }

    /**
     * 测试普通路由规则反转，使用默认验证顺验证失败。
     */
    public function testNormalRuleReverseForDefaultValidatorFail() {
        $rule = $this->_createNormalRule();

        $this->assertFalse($rule->reverse(array(SKIHAT_PARAM_CONTROLLER => 'users123')));
        $this->assertFalse($rule->reverse(array(SKIHAT_PARAM_ACTION => 'edit123')));
        $this->assertFalse($rule->reverse(array(SKIHAT_PARAM_PACKAGE => 'package<123')));
        $this->assertFalse($rule->reverse(array(SKIHAT_PARAM_MODULE => 'module<123')));
        $this->assertFalse($rule->reverse(array(SKIHAT_PARAM_ID => 'abc')));
    }

    /**
     * 测试普通路由规则转换，指定必选值。
     */
    public function testNormalRouterParseForRequired() {
        $rule = $this->_createNormalRule(array(
            'controller' => array(false, false, true)
        ));

        $this->assertFalse($rule->parse(array('action' => 'users')));
        $this->assertNotEmpty($rule->parse(array('controller' => 'users')));
    }

    /**
     * 测试普通路由规则反转，指定必须值。
     */
    public function testNormalRuleReverseForRequired() {
        $rule = $this->_createNormalRule(array(
            'name' => array(false, false, true)
        ));

        $this->assertFalse($rule->reverse(array('controller' => 'users')));
        $this->assertNotEmpty($rule->reverse(array('name' => 'abc')));
    }

    /**
     * 测试普通路由规则转换，指定必选值和默认值。
     */
    public function testNormalRouterParseForRequiredAndDefault() {
        $rule = $this->_createNormalRule(array(
            'controller' => array('index', false, true)
        ));
        $params = $rule->parse();
        $this->assertEquals(1, count($params));
        $this->assertEquals('index', $params['controller']);

        $params = $rule->parse(array('controller' => 'users'));
        $this->assertEquals(1, count($params));
        $this->assertEquals('users', $params['controller']);
    }

    /**
     * 测试普通路由规则反转，指定必须值和默认值。
     */
    public function testNormalRuleReverseForRequiredAndDefault() {
        $rule = $this->_createNormalRule(array(
            'controller' => array('index', false, true)
        ));
        $url = $rule->reverse(array('controller' => 'index'));
        $this->assertEmpty($url);

        $url = $rule->reverse(array('controller' => 'users'));
        $this->assertEquals(1, count($url));
        $this->assertEquals('users', $url['controller']);
    }

    /**
     * 测试普通路由规则反转，指定空值参数。
     */
    public function testNormalRuleReverseForNullFilter() {
        $rule = $this->_createNormalRule();

        $this->assertEmpty($rule->reverse(array('name' => null)));
        $this->assertNotEmpty($rule->reverse(array('name' => 'skihat')));
    }


    /**
     * 测试正则路由规则转换，没有指定正则表达式。
     */
    public function testRegexRuleParseForEmptyExpress() {
        $rule = $this->_createRegexRule();

        $this->assertFalse($rule->parse(array(SKIHAT_PARAM_REWRITE => 'index/users')));
        $this->assertFalse($rule->parse(array('name' => 'skihat')));
    }

    /**
     * 测试正则规则反向转换，空表达式。
     */
    public function testRegexRuleReverseForEmptyExpress() {
        $rule = $this->_createRegexRule();

        $this->assertFalse($rule->reverse(array(SKIHAT_PARAM_REWRITE => 'index/users')));
        $this->assertFalse($rule->reverse(array('name' => 'skihat')));
    }

    /**
     * 测试正则路由规则转换。
     */
    public function testRegexRuleParse() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action'));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit'));

        $this->assertEquals(2, count($params));
        $this->assertEquals('users', $params['controller']);
        $this->assertEquals('edit', $params['action']);
    }

    /**
     * 测试正则规则反向转换。
     */
    public function testRegexRuleReverse() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'edit'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users/edit', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 测试正则路由规则转换，转换失败。
     */
    public function testRegexRuleParseFail() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action'));

        $this->assertFalse($rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit/12')));
        $this->assertFalse($rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit/12', 'name' => 'skihat')));
    }

    /**
     * 测试正则路由规则反转，转换失败。
     */
    public function testRegexRuleReverseFail() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action'));

        $this->assertFalse($rule->reverse(array('controller' => 'users')));
        $this->assertFalse($rule->reverse(array('action' => 'edit')));
        $this->assertFalse($rule->reverse(array('name' => 'skihat')));
    }

    /**
     * 测试正则路由规则转换，指定更多。
     */
    public function testRegexRuleParseForMore() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action'));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit', 'name' => 'skihat'));

        $this->assertEquals(3, count($params));
        $this->assertEquals('users', $params['controller']);
        $this->assertEquals('edit', $params['action']);
        $this->assertEquals('skihat', $params['name']);
    }

    /**
     * 测试正则规则反向转换，包含更多参数，但没有提供Any参数。
     */
    public function testRegexRuleReverseForMore() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'edit', 'name' => 'skihat'));

        $this->assertEquals(2, count($url));
        $this->assertEquals('users/edit', $url[SKIHAT_PARAM_REWRITE]);
        $this->assertEquals('skihat', $url['name']);
    }

    /**
     * 测试正则路则规则转换，指定默认值。
     */
    public function testRegexRuleParseForDefault() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action', 'name' => 'skihat'));

        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit'));
        $this->assertEquals(3, count($params));
        $this->assertEquals('users', $params['controller']);
        $this->assertEquals('edit', $params['action']);
        $this->assertEquals('skihat', $params['name']);

        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit', 'name' => 'skihat 2'));
        $this->assertEquals('skihat 2', $params['name']);
    }

    /**
     * 测试正则路由反转，使用默认值。
     */
    public function testRegexRuleReverseForDefault() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action', 'name' => 'skihat'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'edit', 'name' => 'skihat'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users/edit', $url[SKIHAT_PARAM_REWRITE]);

        $url = $rule->reverse(array('controller' => 'users', 'action' => 'edit', 'name' => 'skihat 2'));
        $this->assertEquals(2, count($url));
        $this->assertEquals('users/edit', $url[SKIHAT_PARAM_REWRITE]);
        $this->assertEquals('skihat 2', $url['name']);
    }

    /**
     * 测试正则规则转换，使用默认验证器。
     */
    public function testRegexRuleParseForDefaultValidatorFail() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action'));

        $this->assertFalse($rule->parse(array(SKIHAT_PARAM_REWRITE => 'users1/abc12')));
        $this->assertFalse($rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit', SKIHAT_PARAM_PACKAGE => 'ab>')));
        $this->assertFalse($rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit', SKIHAT_PARAM_MODULE => 'ab>')));
        $this->assertFalse($rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit', SKIHAT_PARAM_ID => 'abc')));
    }

    /**
     * 测试正则规则反向转换，默认验证器。
     */
    public function testRegexRuleReverseForDefaultValidatorFail() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action'));

        $this->assertFalse($rule->reverse(array('controller' => 'users1', 'action' => 'abc12')));
        $this->assertFalse($rule->reverse(array('controller' => 'users', 'action' => 'edit', SKIHAT_PARAM_PACKAGE => 'abc>')));
        $this->assertFalse($rule->reverse(array('controller' => 'users', 'action' => 'edit', SKIHAT_PARAM_MODULE => 'abc>')));
        $this->assertFalse($rule->reverse(array('controller' => 'users', 'action' => 'edit', SKIHAT_PARAM_ID => 'abc')));
    }

    /**
     * 测试正则规则转换，使用默认验证器。
     */
    public function testRegexRuleParseForDefaultValidator() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action'));

        $this->assertNotEmpty($rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit')));
        $this->assertNotEmpty($rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit', SKIHAT_PARAM_MODULE => 'a1-_.')));
        $this->assertNotEmpty($rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit', SKIHAT_PARAM_PACKAGE => 'a1-_.')));
        $this->assertNotEmpty($rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit', SKIHAT_PARAM_ID => '123')));
    }

    /**
     * 测试正则规则转换，使用默认验证器。
     */
    public function testRegexRuleReverseForDefaultValidator() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action'));

        $this->assertNotEmpty($rule->reverse(array('controller' => 'users', 'action' => 'edit')));
        $this->assertNotEmpty($rule->reverse(array('controller' => 'users', 'action' => 'edit', SKIHAT_PARAM_MODULE => 'a1-_.')));
        $this->assertNotEmpty($rule->reverse(array('controller' => 'users', 'action' => 'edit', SKIHAT_PARAM_PACKAGE => 'a1-_.')));
        $this->assertNotEmpty($rule->reverse(array('controller' => 'users', 'action' => 'edit', SKIHAT_PARAM_ID => '123')));
    }

    /**
     * 测试正则规则，指定验证规则。
     */
    public function testRegexRuleParseForValidator() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action', 'action' => array('index', '[a-z0-9]+')));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/abc12'));

        $this->assertEquals(2, count($params));
        $this->assertEquals('abc12', $params['action']);
    }

    /**
     * 测试正则规则反向转换，指定验证器。
     */
    public function testRegexRuleReverseForValidator() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action', 'action' => array('index', '[a-z0-9]+')));

        $url = $rule->reverse(array('controller' => 'users', 'action' => 'abc12'));
        $this->assertEquals(1, count($url));
        $this->assertEquals('users/abc12', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 测试正则路由规则，首页参数。
     */
    public function testRegexRuleParseForHome() {
        $rule = $this->_createRegexRule(array('express' => '/', 'controller' => array('index', 'index')));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => '/'));

        $this->assertEquals(1, count($params));
        $this->assertEquals('index', $params['controller']);
    }

    /**
     * 测试正则反转，主页
     */
    public function testRegexRuleReverseForHome() {
        # 首页验证过程中，相关参数必须指定验证值，否则反转将会发生错误。
        $rule = $this->_createRegexRule(array('express' => '/', 'controller' => array('index', 'index')));
        $url = $rule->reverse(array('controller' => 'index'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('/', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 测试正则反转，主页的错误用法。
     */
    public function testRegexRuleReverseForHomeFail() {
        $rule = $this->_createRegexRule(array('express' => '/', 'controller' => array('index', 'index')));
        $url = $rule->reverse(array('controller' => 'goods', 'action' => 'index'));

        $this->assertFalse($url);
    }

    /**
     * 测试反转，必须值。
     */
    public function testRegexRuleReverseForRequired() {
        $rule = $this->_createRegexRule(array('express' => '/goods/:action', 'controller' => array(false, 'member', true)));
        $this->assertFalse($rule->reverse(array('action' => 'show')));
    }

    /**
     * 测试正则路由规则转换，指定更多参数。
     */
    public function testRegexRuleParseForMoreParams() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action-:more'));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit-name-skihat-age-12'));

        $this->assertEquals(4, count($params));
        $this->assertEquals('users', $params['controller']);
        $this->assertEquals('edit', $params['action']);
        $this->assertEquals('skihat', $params['name']);
        $this->assertEquals('12', $params['age']);
    }

    /**
     * 测试正则规则反向转换，包含任意参数。
     */
    public function testRegexRuleReverseForMoreParams() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action-:more'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'edit', 'name' => 'skihat', 'age' => '12'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users/edit-name-skihat-age-12', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 测试正则路则规则转换，指定更多参数中包含空值参数。
     */
    public function testRegexRuleParseForMoreParamsHasEmpty() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action-:more'));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit-name--age-12'));

        $this->assertEquals(4, count($params));
        $this->assertEquals('users', $params['controller']);
        $this->assertEquals('edit', $params['action']);
        $this->assertEquals('', $params['name']);
        $this->assertEquals(12, $params['age']);
    }

    /**
     * 测试正则路则规则反转，指定更多参数中包含空值。
     */
    public function testRegexRuleReverseForMoreParamsHasEmpty() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action-:more'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'edit', 'name' => '', 'age' => 12));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users/edit-name--age-12', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 测试正则规则转换，指定更多参数中包含中文字符。
     */
    public function testRegexRuleParseForMoreParamsHasChinese() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action/:more'));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit/name-%E9%9B%AA%E5%B8%BD-age-%E5%8D%81%E4%BA%8C'));

        $this->assertEquals(4, count($params));
        $this->assertEquals('users', $params['controller']);
        $this->assertEquals('edit', $params['action']);
        $this->assertEquals('雪帽', $params['name']);
        $this->assertEquals('十二', $params['age']);
    }

    /**
     * 测试正则规则反转，指定更多参数中包含中文字符。
     */
    public function testRegexRuleReverseForMoreParamsHasChinese() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action/:more'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'edit', 'name' => '雪帽', 'age' => '十二'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users/edit/name-%E9%9B%AA%E5%B8%BD-age-%E5%8D%81%E4%BA%8C', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 测试正则规则转换，指定更多参数包含分隔符。
     */
    public function testRegexRuleParseForMoreParamsAndSeparator() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action-:more.:fmt'));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit-name-skihat-version-2.json'));

        $this->assertEquals(5, count($params));
        $this->assertEquals(2, $params['version']);
        $this->assertEquals('skihat', $params['name']);
        $this->assertEquals('json', $params['fmt']);
    }

    /**
     * 测试正则规则反转，指定更多参数包含分隔符。
     */
    public function testRegexRuleReverseForMoreParamsAndSeparator() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action-:more.:fmt'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'edit', 'name' => 'skihat', 'version' => 2, 'fmt' => 'json'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users/edit-name-skihat-version-2.json', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 测试正则规则转换，指定更多参数包含反线分隔方式。
     */
    public function testRegexRuleParseForMoreParamsAndSeparator1() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action-:more/:fmt'));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit-name-skihat-version-2/json'));

        $this->assertEquals(5, count($params));
        $this->assertEquals(2, $params['version']);
        $this->assertEquals('skihat', $params['name']);
        $this->assertEquals('json', $params['fmt']);
    }

    /**
     * 测试正则规则反转，指定更多参数包含分隔符。
     */
    public function testRegexRuleReverseForMoreParamsAndSeparator2() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action-:more/:fmt'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'edit', 'name' => 'skihat', 'version' => 2, 'fmt' => 'json'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users/edit-name-skihat-version-2/json', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 测试正则规则反向转换，指定验证器，包含任意参数
     */
    public function testRegexRuleParseForMoreParamsValidator() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action-:more', 'name' => array(NULL, '\d+')));

        $this->assertFalse($rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit-name-skihat')));
        $this->assertNotEmpty($rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit-name-123')));
    }

    /**
     * 测试正则规则反向转换，指定验证器，包含任意参数
     */
    public function testRegexRuleReverseForMoreParamsValidator() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action-:more', 'name' => array(NULL, '\d+')));

        $this->assertFalse($rule->reverse(array('controller' => 'users', 'action' => 'edit', 'name' => 'skihat')));
        $this->assertNotEmpty($rule->reverse(array('controller' => 'users', 'action' => 'edit', 'name' => '123')));
    }

    /**
     * 测试正则路由规则，指定可选参数。
     */
    public function testRegexRuleParseForOptional() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action[.:fmt]', 'fmt' => 'html'));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit.json'));

        $this->assertEquals(3, count($params));
        $this->assertEquals('users', $params['controller']);
        $this->assertEquals('edit', $params['action']);
        $this->assertEquals('json', $params['fmt']);
    }

    /**
     * 测试正则规则反转，可选值。
     */
    public function testRegexRuleReverseForOptional() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action[.:fmt]', 'fmt' => 'html'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'edit', 'fmt' => 'json'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users/edit.json', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 测试正则规则转换，使用短参数。
     */
    public function testRegexRuleParseForOptionalAndShortParams() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action[.:fmt]', 'fmt' => 'html'));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit'));

        $this->assertEquals(3, count($params));
        $this->assertEquals('users', $params['controller']);
        $this->assertEquals('edit', $params['action']);
        $this->assertEquals('html', $params['fmt']);
    }

    /**
     * 测试正则规则反转，使用短参数。
     */
    public function testRegexRuleReverseForOptionalAndShortParams() {
        $rule = $this->_createRegexRule(array('express' => ':controller/:action[.:fmt]', 'fmt' => 'html'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'edit', 'fmt' => 'html'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users/edit', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 测试正则路由规则转换，指定最小参数。
     */
    public function testRegexRuleParseForOptionalAndMiniParams() {
        $rule = $this->_createRegexRule(array('express' => ':controller/[:action][.fmt]', 'action' => 'index', 'fmt' => 'html'));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/'));

        $this->assertEquals(3, count($params));
        $this->assertEquals('users', $params['controller']);
        $this->assertEquals('index', $params['action']);
        $this->assertEquals('html', $params['fmt']);
    }

    /**
     * 测试正则路则规则反转，指定最小参数。
     */
    public function testRegexRuleReverseForOptionalAndMiniParams() {
        $rule = $this->_createRegexRule(array('express' => ':controller/[:action][.fmt]', 'action' => 'index', 'fmt' => 'html'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'index', 'fmt' => 'html'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users/', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 测试正则路由规则转换，可选参数失败。
     */
    public function testRegexRuleParseForOptionalAndMiniParams1() {
        $rule = $this->_createRegexRule(array('express' => ':controller/[:action][.:fmt]', 'action' => 'index', 'fmt' => 'html'));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/index.json'));

        $this->assertEquals(3, count($params));
        $this->assertEquals('users', $params['controller']);
        $this->assertEquals('index', $params['action']);
        $this->assertEquals('json', $params['fmt']);
    }

    /**
     * 测试正是路由规则反转，可选参数失败。
     */
    public function testRegexRuleReverseForOptionalAndMiniParams1() {
        $rule = $this->_createRegexRule(array('express' => ':controller/[:action][.:fmt]', 'action' => 'index', 'fmt' => 'html'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'index', 'fmt' => 'json'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users/index.json', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 测试路由转换，可选参数失败。
     */
    public function testRegexRuleParseForOptionalFail() {
        $rule = $this->_createRegexRule(array('express' => ':controller/[:action][.:fmt]', 'action' => 'index', 'fmt' => 'html'));
        $this->assertFalse($rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/.json')));
    }

    /**
     * 测试正则路则规则，指定可选更多参数。
     */
    public function testRegexRuleParseForOptionalAndMoreParams() {
        $rule = $this->_createRegexRule(array('express' => ':controller[/:action][-:more][.:fmt]'));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit-name-skihat.html'));

        $this->assertEquals(4, count($params));
        $this->assertEquals('users', $params['controller']);
        $this->assertEquals('edit', $params['action']);
        $this->assertEquals('skihat', $params['name']);
        $this->assertEquals('html', $params['fmt']);
    }

    /**
     * 测试正则规则反向转换，可选More参数。
     */
    public function testRegexRuleReverseForOptionalAndMoreParams() {
        $rule = $this->_createRegexRule(array('express' => ':controller/[:action][-:more][.:fmt]'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'edit', 'name' => 'skihat', 'fmt' => 'html'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users/edit-name-skihat.html', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 测试正则规则转换，可选项与参数值1。
     */
    public function testRegexRuleParseForOptionalAndMoreParams1() {
        $rule = $this->_createRegexRule(array('express' => ':controller/[:action][-:more][.:fmt]', 'action' => 'index', 'fmt' => 'html'));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/edit.html'));

        $this->assertEquals(3, count($params));
        $this->assertEquals('users', $params['controller']);
        $this->assertEquals('edit', $params['action']);
        $this->assertEquals('html', $params['fmt']);
    }

    /**
     * 测试正则规则反转，可选项与参数值1。
     */
    public function testRegexRuleReverseForOptionalAndMoreParams1() {
        $rule = $this->_createRegexRule(array('express' => ':controller/[:action][-:more][.:fmt]', 'action' => 'index', 'fmt' => 'html'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'edit', 'fmt' => 'html'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users/edit.html', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 测试正则规则转换，可选more参数，短标题。
     */
    public function testRegexRuleParseForOptionalAndMoreParamsAndShort() {
        $rule = $this->_createRegexRule(array('express' => ':controller/[:action][-:more][.:fmt]', 'action' => 'index', 'fmt' => 'html'));
        $params = $rule->parse(array(SKIHAT_PARAM_REWRITE => 'users/'));

        $this->assertEquals(3, count($params));
        $this->assertEquals('users', $params['controller']);
        $this->assertEquals('index', $params['action']);
        $this->assertEquals('html', $params['fmt']);
    }

    /**
     * 测试正则规则反转，可选more参烽，短标题。
     */
    public function testRegexRuleReverseForOptionalAndMoreParamsAndShort() {
        $rule = $this->_createRegexRule(array('express' => ':controller/[:action][-:more][.:fmt]', 'action' => 'index', 'fmt' => 'html'));
        $url = $rule->reverse(array('controller' => 'users', 'action' => 'index', 'fmt' => 'html'));

        $this->assertEquals(1, count($url));
        $this->assertEquals('users/', $url[SKIHAT_PARAM_REWRITE]);
    }

    /**
     * 根据$ioc创建一般规则，并返回相关实例。
     *
     * @param string|array $props Ioc配置值。
     *
     * @return NormalRouterRule
     */
    protected function _createNormalRule($props = array()) {
        return Skihat::ioc(array_merge(array(SKIHAT_IOC_CLASS => '#NormalRouterRule'), $props));
    }

    /**
     * 根据$props参数创建正则路由规则实例并返回。
     *
     * @param array $props Ioc属性值。
     *
     * @return RegexRouterRule
     */
    protected function _createRegexRule($props = array()) {
        return Skihat::ioc(array_merge(array(SKIHAT_IOC_CLASS => '#RegexRouterRule'), $props));
    }
}