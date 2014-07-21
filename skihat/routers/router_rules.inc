<?php
/**
 * router_rules.inc 路由规则样式文件，提供路由规则接口声明。
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

/**
 * 路由规则接口，提供生成不同路上规范服务。
 *
 * @package skihat.routers
 */
interface IRouterRule {
    /**
     * 从$params参数中转换出完整的URL参数集合，转换失败返回false。
     *
     * @param array $params 转换参数。
     *
     * @return mixed
     */
    function parse(array $params = array());

    /**
     * 从$params参数中反向转换出URL所需要参数集合，转换失败返回false。
     *
     * @param array $params 转换参数。
     *
     * @return mixed
     */
    function reverse(array $params = array());
}

/**
 * 抽象路由规则，提供路由规则的基本服务。
 *
 * @package skihat.routers
 */
abstract class AbstractRouterRule implements IRouterRule {
    /**
     * @const string 语法名称验证器，验证普通命名规则。
     */
    const SYNTAX_NAME_VALIDATOR = '[a-z_]+';

    /**
     * @const string 普通命名验证器，提供更多的命名选项。
     */
    const NORMAL_NAME_VALIDATOR = '[a-z0-9_\-.]+';

    /**
     * @const string 数字验证器，提供数字验证规则。
     */
    const NUMBER_VALIDATOR = '\d+';

    /**
     * @see IRouterRule::parse
     */
    public function parse(array $params = array()) {
        # 如果转换参数失败，直接返回false。
        $fullParams = $this->_fullParse($params);

        # 如果转换完整参数成功。
        if ($fullParams !== false) {
            $fullParams = array_merge($this->_defaults, $fullParams);

            if (!$this->_validateRequires($fullParams)) return false;
            if (!$this->_validateValidators($fullParams)) return false;

            return $fullParams;
        }

        return false;
    }

    /**
     * @see IRouterRule::reverse
     */
    public function reverse(array $params = array()) {
        # 合并默认参数值，并过滤无效参数。
        $fullParams = array_merge($this->_defaults, $params);
        $fullParams = $this->_invalidFilter($fullParams);

        # 检查必须值和验证值的有效性。
        if (!$this->_validateRequires($fullParams)) return false;
        if (!$this->_validateValidators($fullParams)) return false;

        # 如果完整返转失败，直接返回false。
        return $this->_fullReverse($fullParams);
    }

    /**
     * PHP 魔术方法，提供规则参数选项，允许指定默认值，有效性验证和必选参数。
     *
     * @example
     *      $rule = new NormalRouterStyle();
     *      $rule->controller = 'xxx';                   # 指定参数controller的默认值为xxx;
     *      $rule->controller = array('xxx');            # 指定参数controller的默认值为xxx;
     *      $rule->controller = array('xxx','xx1');      # 指定参数controller的有效性规则为xx1(正则验证);
     *      $rule->controller = array('xxx','xx1',true); # 指定参数controller为必填参数。
     *
     * @param string $name 参数名称。
     * @param mixed  $val  参数选项。
     */
    public function __set($name, $val) {
        # 将值转换为数组格式。
        $val = (array)$val;

        if (!empty($val[0])) $this->_defaults[$name] = $val[0];
        if (!empty($val[1])) $this->_validators[$name] = $val[1];
        if (!empty($val[2])) array_push($this->_requires, $name);
    }

    /**
     * 根据$params参数，转换出完整的参数集合，如果转换返回返回false。
     *
     * @param array $params 参数集合。
     *
     * @return mixed
     */
    protected abstract function _fullParse(array &$params);

    /**
     * 根据$params参数，返转出完整的参数集合，如果返转失败返回false。
     *
     * @param array $params 参数集合。
     *
     * @return mixed
     */
    protected abstract function _fullReverse(array &$params);

    /**
     * 参数过滤，验证并过滤无效的参数值，并返回有效参数值。
     *
     * @param array $params 验数集。
     *
     * @return array
     */
    protected function _invalidFilter(array &$params) {
        return array_filter($params, 'not_null');
    }

    /**
     * 根据$params进行必须验证，验证成功返回true，否则返回false。
     *
     * @param array $params 需要验证的参数集。
     *
     * @return bool
     */
    protected function _validateRequires(array &$params) {
        # 从必须值中获取参数名称，判断是否在参数中存在。
        foreach ($this->_requires as $name) {
            if (!isset($params[$name])) return false;
        }

        return true;
    }

    /**
     * 根据$params进行参数有效性验证，验证成功返回true，失败返回false。
     *
     * @param array $params 需要验证的参数集。
     *
     * @return bool
     */
    protected function _validateValidators(array &$params) {
        # 从验证器中获取参数名称和验证表达式，如果需要验证的参数不存在，直接处理一下个。
        foreach ($this->_validators as $name => $validator) {
            if (!isset($params[$name])) continue;
            if (!preg_match('/^' . $validator . '$/', $params[$name])) return false;
        }

        return true;
    }

    /**
     * @var array 参数默认值。
     */
    protected $_defaults = array();

    /**
     * @var array 参数必选值。
     */
    protected $_requires = array();


    /**
     * @var array 参数有效性验证值，提供有默认模块，包，控制器，活动和ID的选项。
     */
    protected $_validators = array(
        SKIHAT_PARAM_CONTROLLER => self::SYNTAX_NAME_VALIDATOR,
        SKIHAT_PARAM_PACKAGE => self::NORMAL_NAME_VALIDATOR,
        SKIHAT_PARAM_MODULE => self::NORMAL_NAME_VALIDATOR,
        SKIHAT_PARAM_ACTION => self::SYNTAX_NAME_VALIDATOR,
        SKIHAT_PARAM_ID => self::NUMBER_VALIDATOR
    );
}

/**
 * 普通路由规则，提供对一般请求参数的路由规则转换。
 *
 * @package skihat.routers
 */
class NormalRouterRule extends AbstractRouterRule {
    /**
     * @see AbstractRouterRule::_fullParse
     */
    protected function _fullParse(array &$params) {
        return $params;
    }

    /**
     * @see AbstractRouterRule::_fullReverse
     */
    protected function _fullReverse(array &$params) {
        return array_diff_assoc($params, $this->_defaults);
    }
}

/**
 * 正则表达式路由规则，提供通过正则表达式转换和处理参数的服务。
 *
 * @package skihat.routers
 */
class RegexRouterRule extends AbstractRouterRule {
    /**
     * @const string 更多参数选项。
     */
    const MORE_PARAM = 'more';

    /**
     * @const string 更多参数验证器。
     */
    const MORE_VALIDATOR = '[^.\/]+';

    /**
     * @const string 更多参数分隔符。
     */
    const MORE_SEPARATOR = '-';

    /**
     * @const string 首页地址。
     */
    const HOME_URL = '/';

    /**
     * @var string|bool 正则表达式的值，默认为空值。
     */
    public $express = false;

    /**
     * 构造方法，提供RegexRouterRule实例初始化。
     */
    public function __construct() {
        $this->_validators[self::MORE_PARAM] = self::MORE_VALIDATOR;
    }

    /**
     * @see AbstractRouterRule::_fullParse
     */
    protected function _fullParse(array &$params) {
        # 如果没有正确设置正则表达式，直接返回false值。
        if (!$this->express) return false;

        # 读取重写参数值，如果不存在则表示使用首页URL请求。
        if (($rewriteParams = $this->_parseRewrite($params)) !== false) {
            # 如果包含更多处理参数。
            if ($more = array_key_pop($rewriteParams, self::MORE_PARAM, false)) {
                $rewriteParams = array_merge($this->_parseMore($more), $rewriteParams);
            }

            return array_merge($this->_defaults, $params, $rewriteParams);
        }

        return false;
    }

    /**
     * 根据重写参数转换出参数信息，如果转换失败返回false。
     *
     * @param array $params 参数值。
     *
     * @return array|bool
     */
    protected function _parseRewrite(array &$params) {
        $rewrite = array_key_pop($params, SKIHAT_PARAM_REWRITE, self::HOME_URL);
        $matches = array();

        # 如果正则匹配成功，则处理并返回。
        if (preg_match_all($this->_regexPattern(), $rewrite, $matches, PREG_SET_ORDER)) {
            return array_key_filter($matches[0], 'not_numeric');
        }

        return false;
    }

    /**
     * 根据$more参数，转换更多参数值并返回。
     *
     * @param string $more 更多参数值。
     *
     * @return array
     */
    protected function _parseMore($more) {
        # 如果更多值的第一个值为'/'值。
        $more = explode(self::MORE_SEPARATOR, $more);
        $length = count($more);
        $params = array();

        for ($i = 0; $i < $length; $i += 2) {
            $params[urldecode($more[$i])] = urldecode($more[$i + 1]);
        }

        return $params;
    }

    /**
     * @see AbstractRouterRule::_fullReverse
     */
    protected function _fullReverse(array &$params) {
        # 如果没有指定验证器，直接返回false。
        if (!$this->express) return false;

        $pattern = $this->_regexPattern();
        $currParams = $params;
        $replace = array();

        foreach ($this->_regexParams as $name => $syntaxName) {
            $replace[$syntaxName] = array_key_pop($currParams, $name, '');
        }

        $currParams = array_diff_assoc($currParams, $this->_defaults);
        if (!empty($currParams) && isset($this->_regexParams[self::MORE_PARAM])) {
            $replace[':' . self::MORE_PARAM] = $this->_reverseMore($currParams);
            $currParams = array();
        }

        # 生成完整URL参数，并且处理默认值。
        $url = str_replace(array_keys($replace), array_values($replace), $this->express);
        if (empty($currParams) && !array_diff_assoc($this->_defaults, $params) && empty($replace[':' . self::MORE_PARAM])) {
            $url = preg_replace('/\[\S+\]|\[\]/', '', $url);
        } else {
            $url = str_replace(array('[-]', '[/]', '[]', '[', ']'), '', $url);
        }

        # 如果URL验证成功。
        if (preg_match($pattern, $url)) {
            $currParams[SKIHAT_PARAM_REWRITE] = $url;
            return $currParams;
        }

        return false;
    }

    /**
     * 根据$params生成返转成更多参数选项。
     *
     * @param array $params 参数集。
     *
     * @return string
     */
    protected function _reverseMore(array &$params) {
        return strtr(http_build_query($params, NULL, self::MORE_SEPARATOR), '=', '-');
    }

    /**
     * 根据$express成员变量，返回当当前正则模式。
     *
     * @return string
     */
    protected function _regexPattern() {
        #  如果正则表达式还未生成。
        if (!$this->_regexPattern) {
            $pattern = preg_replace_callback('/\\\:([a-z]+)/', array($this, '_regexNameReplace'), preg_quote($this->express, '/'));

            # 处理可选参数，并将类型设置为正则类型。
            $pattern = str_replace(array('\\[', '\\]'), array('(', '){0,1}'), $pattern);
            $this->_regexPattern = ('/^' . $pattern . '$/');
        }

        return $this->_regexPattern;
    }

    /**
     * 正则参数名称替换，将指定名称替换为正则表达式模式。
     *
     * @param array $names 匹配出的参数名称集合。
     *
     * @return string
     */
    protected function _regexNameReplace($names) {
        # 获取参数的命名部分，并设置为正则参数集合。
        $name = $names[1];
        $this->_regexParams[$name] = ':' . $name;

        # 获取验证器，如果验证器不存在则使用默认验证器[a-z0-9+]。
        $validator = isset($this->_validators[$name]) ? $this->_validators[$name] : '[a-z0-9_]+';
        return "(?P<$name>" . $validator . ')';
    }

    /**
     * @var bool|string 正则表达式模式。
     */
    protected $_regexPattern = false;

    /**
     * @var array 正则表达式参数。
     */
    protected $_regexParams = array();
}

/**
 * Restful 路由规则，提供对RESTFUL架构的支持。
 *
 * @package skihat.routers
 */
class RestfulRouterRule extends RegexRouterRule {

}