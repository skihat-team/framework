<?php
/**
 * security_test.inc 提供安全组件接口单元测试声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.securities
 * @version   1.0
 *
 * @created   2012-06-12
 * @modified  2013-08-02
 */

# 引用测试启动文件
require_once(dirname(dirname(dirname(dirname(__FILE__)))) . '/startup.inc');

# 引入被测试类
Skihat::import('kernels.securities.security', SKIHAT_PATH_LIBRARY);

/**
 * 安全接口测试类，提供对安全组件接口的测试服务。
 *
 * @package skihat.kernels.securities
 */
class SecurityTest extends PHPUnit_Framework_TestCase {

    /**
     * 测试安全验证。
     */
    public function testAuthenticate() {
        $user = Security::authenticate(array('skihat', '123321'));

        $this->assertNotEmpty($user);
        $this->assertEquals(1, $user['id']);
        $this->assertEquals('skihat', $user['name']);
    }

    /**
     * 测试验证失败。
     */
    public function testAuthenticateFail() {
        $user = Security::authenticate(array('skihat', 'error'));

        $this->assertEmpty($user);
    }

    /**
     * 测试验证没有提供。
     *
     * @expectedException ConfigException
     */
    public function testAuthenticationNotSupport() {
        Security::authenticate(array('skihat', '123321'), 'not-exists');
    }

    /**
     * 测试授权。
     */
    public function testAuthorize() {
        $this->assertTrue(Security::authorize(array('id' => 1, 'name' => 'skihat'), 'users'));
    }

    /**
     * 测试授权失败。
     */
    public function testAuthorizeFail() {
        $this->assertFalse(Security::authorize(array('id' => 1, 'name' => 'error'), 'users'));
        $this->assertFalse(Security::authorize(array('id' => 1, 'name' => 'skihat'), 'error'));
        $this->assertFalse(Security::authorize(array('id' => 1, 'name' => 'skihat'), 'users', 'fail'));
    }

    /**
     * 测试授权没有提供。
     *
     * @expectedException ConfigException
     */
    public function testAuthorizeNotSupport() {
        Security::authorize(array('id' => 1, 'name' => 'skihat'), 'users', 'sum', 'not-exists');
    }

    public function setUp() {
        Skihat::write(Security::CONFIG_NAME, array(
            Security::CONFIG_AUTHENTICATIONS => array(
                'default' => '#AuthenticationMock'
            ),
            Security::CONFIG_AUTHORIZATIONS => array(
                'default' => '#AuthorizationMock'
            )
        ));

        Security::initialize();
    }
}

class AuthenticationMock implements IAuthentication {
    public function authenticate($identity) {
        list ($name, $password) = $identity;

        if ($name == 'skihat' && $password == '123321') {
            return array('id' => 1, 'name' => 'skihat');
        }

        return false;
    }
}

class AuthorizationMock implements IAuthorization {
    public function authorize($identity, $sro, $rule = self::RULE_SUM) {
        if ($identity['id'] == 1 && $identity['name'] == 'skihat') {
            return $sro == 'users' && $rule == self::RULE_SUM;
        }

        return false;
    }
}
