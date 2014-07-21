<?php
/**
 * router.inc  路由组件接口声明文件，提供路由组件接口声明。
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

# 导入预定的路由样式和路由规则。
Skihat::import('routers.router_styles', SKIHAT_PATH_LIBRARY);
Skihat::import('routers.router_rules', SKIHAT_PATH_LIBRARY);

/**
 * 路由组件异常，当路由发生错误时引发本异常。
 *
 * @package skihat.routers
 */
class RouterException extends SkihatException {
}

/**
 * 路由组件接口类，提供路由服务所需的服务支持。
 */
class Router {
    /**
     * @const string 完整URL标识。
     */
    const FULL_URL = 'fullUrl';

    /**
     * @const string 规则名称常量。
     */
    const RULE_NAME = 'ruleName';

    /**
     * 根据$basicParams和$fullParams初始化默认参数值。
     *
     * @param array $basicParams 基本参数。
     * @param array $fullParams  完整参数。
     */
    public static function defaultParams(array $basicParams, array $fullParams) {
        self::$_basicParams = $basicParams;
        self::$_fullParams = $fullParams;
    }

    /**
     * 根据$domain返回该域名下的路由实例。
     *
     * @param string $domain 域名（例如：www.example.com）。
     *
     * @return Router
     */
    public static function domain($domain) {
        # 如果当前域名的相关路由还不存在。
        if (isset(self::$_routers[$domain]) == false) {
            self::$_routers[$domain] = new Router($domain);
        }

        return self::$_routers[$domain];
    }

    /**
     * 根据$params参数，匹配出完整的参数集合。
     *
     * @param array $params 参数集合。
     *
     * @return mixed
     */
    public static function match(array $params) {
        # 根据当前域名获取全部路由规则。
        $rules = & self::_linkRules(SKIHAT_CURRENT_DOMAIN);
        # 进行规则匹配。
        foreach ($rules as &$rule) {
            $objRule = self::_createRuleObject($rule);
            # 如果转换参数成功，返回转换后的参数集合。
            if (($allParams = $objRule->parse($params)) !== false) {
                return $allParams;
            }
        }

        return false;
    }

    /**
     * 根据$url生成完整URL地址并返回，如果地址生成失败返回false。
     *
     * @param array $params URL参数信息。
     *
     * @throws RouterException
     * @return string|bool
     */
    public static function url(array $params = array()) {
        # 如果当前参数中没有指定规则名称，则附加默认参数或完整参数。
        if (!($ruleName = array_key_pop($params, self::RULE_NAME, false))) {
            if ($fullParams = array_key_pop($params, self::FULL_URL)) {
                $params = array_merge(self::$_fullParams, $params);
            } else {
                $params = array_merge(self::$_basicParams, $params);
            }

            $ruleName = SKIHAT_CURRENT_DOMAIN;
        }

        # 获取指定规则的全部规则集合。
        $rules = self::_linkRules($ruleName);
        $params = array_filter($params,'not_empty');
        foreach ($rules as & $rule) {
            $objRule = self::_createRuleObject($rule);
            if (($allParams = $objRule->reverse($params)) !== false) {
                return $rule['router']->_reverseUrl($allParams);
            }
        }

        if (SKIHAT_DEBUG) { # 如果当前为调试状态。
            throw new RouterException('Router::not match rule.');
        }

        return false;
    }

    /**
     * 根据$domain初始化Router实例（保护方法防止外部实例化）。
     *
     * @param string $domain 域名。
     */
    protected function __construct($domain) {
        $this->_domain = $domain;
    }

    /**
     * 根据$style指定当前路由的Ioc配置信息，并返回路由实例。
     *
     * @param mixed $style 样式IOC配置。
     *
     * @return Router
     */
    public function style($style) {
        $this->_style = $style;
        return $this;
    }

    /**
     * 根据$rule和$names增加路由规则，如果名称为false表示匿名规则。
     *
     * @param mixed $rule  规则IOC配置。
     * @param mixed $names 规则名称，允许使用数组指定多个值。
     *
     * @return Router
     */
    public function rule($rule, $names = false) {
        # 将规则增加到全局规则表中。
        $currRule = self::_addRule($this, $rule);
        # 附加到当前规则中链中去。
        self::_addLinkRule($this->_domain, $currRule);

        # 附加命名规则。
        if (!$names) return $this;
        foreach ((array)$names as $name) {
            self::_addLinkRule($name, $currRule);
        }

        return $this;
    }

    /**
     * 根据$params参数值，返转生成URL地址。
     *
     * @param array $params 参数值。
     *
     * @return string
     */
    protected function _reverseUrl(array $params) {
        # 如果当前路由样式实例对象还没有创建，初始化实例。
        if (!$this->_objStyle) {
            $this->_objStyle = Skihat::ioc($this->_style);
        }

        return $this->_objStyle->fullUrl($params);
    }

    /**
     * 根据$router和$rule注册新的路则规则集合，并返回规则引用。
     *
     * @param Router $router 路由实例。
     * @param mixed  $rule   路由规则IOC参数值。
     *
     * @return mixed
     */
    protected static function & _addRule(Router $router, $rule) {
        $count = array_push(self::$_rules, array('rule' => $rule, 'router' => $router, 'objRule' => false));
        return self::$_rules[$count - 1];
    }

    /**
     * 根据$rule创建规则实例对象。
     *
     * @param string $rule 规则Ioc配置。
     *
     * @return IRouterRule
     */
    protected static function _createRuleObject(&$rule) {
        #  如果当前规则的实例还没有创建。
        if (!$rule['objRule']) {
            $rule['objRule'] = Skihat::ioc($rule['rule']);
        }

        return $rule['objRule'];
    }

    /**
     * 根据$name和$rule附加新的路由规则链。
     *
     * @param string $name 规则链名称。
     * @param mixed  $rule 规则引用地址。
     */
    protected static function _addLinkRule($name, &$rule) {
        # 如果当前链名称是第一次附加。
        if (isset(self::$_linkRules[$name]) == false) {
            self::$_linkRules[$name] = array($rule);
        } else {
            self::$_linkRules[$name][] = $rule;
        }
    }

    /**
     * 根据$name获取路由规则集合。
     *
     * @param string $name 路由链名称。
     *
     * @return array
     */
    protected static function & _linkRules($name) {
        # 如果当前命名的规则不存在，则创建新的值。
        if (!isset(self::$_linkRules[$name])) {
            self::$_linkRules[$name] = array();
        }

        return self::$_linkRules[$name];
    }

    /**
     * @var bool|string 路由实例的域名值。
     */
    protected $_domain = false;

    /**
     * @var string|array 路由样式Ioc配置值。
     */
    protected $_style = '#NormalRouterStyle';

    /**
     * @var IRouterStyle 路由样式实例。
     */
    protected $_objStyle = false;

    /**
     * @var array 基本参数集合，生成URL时会自动附加基本参数集合。
     */
    protected static $_basicParams = array();

    /**
     * @var array 完整参数集合，生成URL时如果指定完整URL则会附加完整参数。
     */
    protected static $_fullParams = array();

    /**
     * @var array 全部路由实例集合。
     */
    protected static $_routers = array();

    /**
     * @var array 全部路由规则集合。
     */
    protected static $_rules = array();

    /**
     * @var array 规则链集合。
     */
    protected static $_linkRules = array();
}