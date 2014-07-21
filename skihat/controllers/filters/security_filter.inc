<?php
/**
 * security_filter.inc 提供控制器的安全过滤支持。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.controllers.filters
 * @version   1.0
 *
 * @created   2013-04-08
 * @modified  2013-09-18
 */

Skihat::import('kernels.securities.security', SKIHAT_PATH_LIBRARY);

/**
 * 安全过滤支持，提供对控制器的安全支持。
 *
 * @package skihat.controllers.filters
 */
class SecurityFilter implements IActionFilter {
    /**
     * @var mixed 指定验证的活动集合，'*'表示全部。
     */
    public $authenticates = false;

    /**
     * @var mixed 指定授权的活动集合。
     */
    public $authorizes = false;

    /**
     * @var string 使用的验证名称。
     */
    public $authentication = 'default';

    /**
     * @var string 使用的授权名称。
     */
    public $authorization = 'default';

    /**
     * @var string 默认验证完成URL地址。
     */
    public $defaultUrl = '/';

    /**
     * @var string 登录URL地址。
     */
    public $loginUrl = '/';

    /**
     * 返回当前的验证用户信息，如果还没有验证将返回false。
     *
     * @return mixed
     */
    public function identity() {
        return array_key_value($_SESSION, self::_securitySession(), false);
    }

    /**
     * 根据$identity进行安全验证，验证成功返回true，失败返回false。
     *
     * @param mixed $identity     用户的标识信息。
     * @param bool  $autoRedirect 是否自动返回要求验证的地址。
     *
     * @return bool
     */
    public function authenticate($identity, $autoRedirect = true) {
        # 如果当前用户验证成功
        if ($identity = Security::authenticate($identity, $this->authentication)) {
            # 注册身份证明。
            $this->register($identity);

            # 如果执行自动跳转。
            if ($autoRedirect) {
                $this->autoRedirect();
            }

            return true;
        }

        return false;
    }

    /**
     * 根据$sro和$rule进行安全认识，并返回认证的结果。
     *
     * @param string $sro  安全资源对象。
     * @param string $rule 验证规则。
     *
     * @return mixed
     */
    public function authorize($sro, $rule = 'sum') {
        # 获取当前验证信息，如果验证信息为空，直接全部返回false。
        if ($identity = $this->identity()) {
            return Security::authorize($identity, $sro, $rule, $this->authorization);
        }

        # 如果sro为数组，直接返回数组填充值，否则返回false。
        return is_array($sro) ? array_fill(0, count($sro), false) : false;
    }

    /**
     * 将$identity注册为当前用户的标识信息。
     *
     * @param $identity
     */
    public function register($identity) {
        $_SESSION[self::_securitySession()] = $identity;
    }

    /**
     * 注销当前用户的验证。
     */
    public function unRegister() {
        unset($_SESSION[self::_securitySession()]);
        session_regenerate_id();
    }

    /**
     * 根据SESSION信息执行自动跳转。
     */
    public function autoRedirect() {
        $returnUrl = array_key_pop($_SESSION, $this->_returnSession(), $this->defaultUrl);
        $this->_controller->redirect($returnUrl);
    }

    /**
     * @see IActionFilter::initialize
     */
    public function initialize(Controller $controller) {
        $this->_controller = $controller;
    }

    /**
     * @see IActionFilter::invoke
     */
    public function invoke($actionMethod) {
        # 将用户标识设置给当前控制器。
        $this->_controller['identity'] = $this->identity();

        # 如果当前安全验证失败。
        if (!$this->_authenticate($actionMethod)) {
            # 记录最后访问的地址，并且跳转到登录地址。
            $_SESSION[self::_returnSession()] = client_url();
            $this->_controller->message('对不起，继续前请登录账户。', SKIHAT_MESSAGE_INFO);
            $this->_controller->redirect($this->loginUrl);
            return false;
        }

        # 如果授权验证失败。
        if ($this->identity() && !$this->_authorize($actionMethod)) {
            $this->_controller->error(402);
            return false;
        }

        return true;
    }

    /**
     * @see IActionFilter::render
     */
    public function render(array &$options) {
        return TRUE;
    }

    /**
     * @see IActionFilter::complete
     */
    public function complete() {

    }

    /**
     * 根据活动名称判断当前是否需要进行身份验证。
     *
     * @param string $actionMethod 活动方法名称 。
     *
     * @return bool
     */
    protected function _authenticate($actionMethod) {
        # 如果不需要验证直接返回。
        if (!$this->authenticates) return true;
        # 如果当前活动需要验证，并且还没有进行验证。
        if ($this->authenticates == '*' || in_array($actionMethod, (array)$this->authenticates)) {
            # 如果当前没有身份标识，记录当前地址，并返回false。
            if ($this->identity() == false) {
                return false;
            }
        }

        return true;
    }

    /**
     * 根据活动名称判断，是否授权验通过。
     *
     * @param string $actionMethod 活动方法。
     *
     * @return bool
     */
    protected function _authorize($actionMethod) {
        # 如果使用*表示验证所有的活动。
        if (!$this->authorizes) return true;
        if ($this->authorizes == '*') return $this->_defaultAuthorize();
        if ($sro = array_key_value($this->authorizes, $actionMethod)) {
            # 如果验证的资源值为*或true表示使用默认资源标识的生成方式。
            if ($sro == '*' || $sro === true) return $this->_defaultAuthorize();

            # 根据安全资源生成结果。
            $rights = Security::authorize($this->identity(), $sro, 'sum', $this->authorization);
            return array_sum($rights) > count($rights);
        }

        return true;
    }

    /**
     * @return string 安全SESSION关键字。
     */
    protected function _securitySession() {
        return 'security-filter-' . $this->authentication . '-' . $this->authorization;
    }

    /**
     * @return string 登录完成后的返回地址。
     */
    protected function _returnSession() {
        return 'security-return-' . $this->authorization . '-' . $this->authorization;
    }

    /**
     * 返回当前请求的默认验证结果。
     */
    protected function _defaultAuthorize() {
        $rights = Security::authorize($this->identity(), $this->_defaultSro(), 'sum');
        return array_sum($rights) > count($rights);
    }

    /**
     * 返回默认Sro验证。
     *
     * @return array
     */
    protected function _defaultSro() {
        # 获取当前请求实例。
        $request = $this->_controller->request;

        $module = $request->module('global');
        $controller = $request->controller();
        $action = $request->action();

        return array(
            $module . '-' . $controller . '-' . $action,
            $module . '-' . $controller, $module
        );
    }

    /**
     * @var Controller 控制器。
     */
    protected $_controller = null;
}