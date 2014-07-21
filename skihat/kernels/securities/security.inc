<?php
/**
 * security.inc 提供安全组件接口声明。
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

/**
 * 安全组件异常，提供安全服务的异常服务。
 *
 * @package skihat.kernels.securities
 */
class SecurityException extends SkihatException {
}

/**
 * 身份认证接口，提供用户身份认证服务。
 *
 * @package skihat.kernels.securities
 */
interface IAuthentication {
    /**
     * 根据$identity验证用户身份，验证成功返回用户标识信息，失败返回false。
     *
     * @param mixed $identity 身份证明。
     *
     * @return mixed
     */
    function authenticate($identity);
}

/**
 * 安全授权接口，提供安全授权服务。
 *
 * @package skihat.kernels.securities
 */
interface IAuthorization {

    /**
     * @const string 验证规则，和值。
     */
    const RULE_SUM = 'sum';

    /**
     * @const string 验证规则，最小值。
     */
    const RULE_MIN = 'min';

    /**
     * @const string 验证规则，最大值。
     */
    const RULE_MAX = 'max';

    /**
     * @const string 验证规则，集合。
     */
    const RULE_RANGE = 'range';

    /**
     * 根据$identity,$sro和$rule获取授权信息。
     *
     * @param mixed  $identity 用户标识。
     * @param mixed  $sro      安全对象标识。
     * @param string $rule     安全规则。
     *
     * @return mixed
     */
    function authorize($identity, $sro, $rule = self::RULE_SUM);
}

/**
 * 安全组件接口，提供安全检查服务。
 *
 * @package skihat.kernels.securities
 */
abstract class Security {
    /**
     * @const string 安全调试名称。
     */
    const DEBUG_NAME = 'kernels/securities';

    /**
     * @const string 安全组件配置名称。
     */
    const CONFIG_NAME = 'kernels/securities';

    /**
     * @const string 验证配设置名称。
     */
    const CONFIG_AUTHENTICATIONS = 'authentications';

    /**
     * @const string 授权配置名称。
     */
    const CONFIG_AUTHORIZATIONS = 'authorizations';

    /**
     * @const string 默认验证名称。
     */
    const DEFAULT_AUTHENTICATION = 'default';

    /**
     * @const string  默认授权名称。
     */
    const DEFAULT_AUTHORIZATION = 'default';

    /**
     * 初始化安全组件。
     */
    public static function initialize() {
        # 读取动态配置信息。
        $conf = Skihat::read(self::CONFIG_NAME, array(
            self::CONFIG_AUTHENTICATIONS => array(),
            self::CONFIG_AUTHORIZATIONS => array()));

        # 创建工厂实例。
        self::$_authenticationFactory = new IocFactory($conf[self::CONFIG_AUTHENTICATIONS]);
        self::$_authorizationFactory = new IocFactory($conf[self::CONFIG_AUTHORIZATIONS]);
    }

    /**
     * 根据$identity和$authName进行身份验证，成功返回验证的对象信息，失败返回false。
     *
     * @param mixed  $identity 进行验证的身份标识。
     * @param string $authName 验证器的名称，默认为default。
     *
     * @return mixed|bool
     */
    public static function authenticate($identity, $authName = self::DEFAULT_AUTHENTICATION) {
        $authentication = self::$_authenticationFactory->instance($authName);
        return $authentication->authenticate($identity);
    }

    /**
     * 根据$identity、$sro、$rule和$authName进行授权检查。
     *
     * @param mixed  $identity 身份证明。
     * @param mixed  $sro      安全资源对象。
     * @param string $rule     授权规则。
     * @param string $authName 授权名称，默认为default。
     *
     * @return mixed
     */
    public static function authorize($identity, $sro, $rule = IAuthorization::RULE_SUM,
                                     $authName = self::DEFAULT_AUTHORIZATION) {
        $authorization = self::$_authorizationFactory->instance($authName);
        return $authorization->authorize($identity, $sro, $rule);
    }

    /**
     * @var IocFactory 提供安全验证工厂实例。
     */
    protected static $_authenticationFactory = null;

    /**
     * @var IocFactory 提供安全授权工厂实例。
     */
    protected static $_authorizationFactory = null;
}

Security::initialize();