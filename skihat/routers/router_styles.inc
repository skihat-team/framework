<?php
/**
 * router_styles.inc 路由样式文件，提供路由样式接口声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.routers
 * @version   1.0
 *
 * @created   2012-08-02
 * @modified  2013-08-22
 */

/**
 * 路由风格接口，提供生成不同URL的方案支持。
 *
 * @package skihat.routers
 */
interface IRouterStyle {

    /**
     * @const string 默认脚本名称。
     */
    const DEFAULT_SCRIPT = 'index.php';

    /**
     * 根据$params参数生成完整的URL地址。
     *
     * @param array $params 参数集合。
     *
     * @return mixed
     */
    function fullUrl(array $params = array());
}

/**
 * 抽象路由风格，提供路由风格所需的基础服务。
 *
 * @package skihat.routers
 */
abstract class AbstractRouterStyle implements IRouterStyle {
    /**
     * @var string|bool 完整域名信息，如果设置为false，则表示不生成完整域名。
     */
    public $domain = false;

    /**
     * @see IRouterStyle::fullUrl
     */
    public function fullUrl(array $params = array()) {
        #  获取锚和路径URL参数。
        $anchor = $this->_anchorUrl($params);
        $path = $this->_pathUrl($params);

        # 如果参数不为空，则生成参数值。
        if (!empty($params)) {
            $path .= '?' . http_build_query($params);
        }

        return $this->_domainUrl() . $path . $anchor;
    }

    /**
     * 根据当前实例域名信息返回相关的URL地址。
     */
    protected function _domainUrl() {
        return $this->domain ? $this->domain . '/' : '/';
    }

    /**
     * 根据$params处理URL路径信息，并返回路径URL地址。
     *
     * @param array $params URL参数集合。
     *
     * @return string
     */
    protected abstract function _pathUrl(array &$params);

    /**
     * 根据$params生成URL锚参数地址。
     *
     * @param array $params URL参数集。
     *
     * @return string
     */
    protected function _anchorUrl(array &$params) {
        # 如果在参数中包含锚信息，则返回生成内容。
        if ($anchor = array_key_pop($params, SKIHAT_PARAM_ANCHOR)) {
            return '#' . urlencode($anchor);
        }

        return '';
    }
}

/**
 * 普通路由样式，提供对一般路由样式的支持。
 *
 * @package skihat.routers
 */
class NormalRouterStyle extends AbstractRouterStyle {
    /**
     * @var string 脚本文件名称，默认为index.php。
     */
    public $script = self::DEFAULT_SCRIPT;

    /**
     * @see AbstractRouter::_pathUr
     */
    protected function _pathUrl(array &$params) {
        return ($this->script != self::DEFAULT_SCRIPT) ? $this->script : '';
    }
}


/**
 * 重写路由样式，生成满足重写规则的样式支持。
 *
 * @package skihat.routers
 */
class RewriteRouterStyle extends AbstractRouterStyle {
    /**
     * @see AbstractRouterStyle::_pathUrl
     */
    protected function _pathUrl(array &$params) {
        # 如果在数组中包含重写参数。
        if ($rewriteUrl = array_key_pop($params, SKIHAT_PARAM_REWRITE)) {
            return $rewriteUrl;
        }

        return '';
    }
}

/**
 * PHP Pathinfo 路由样式，生成满足pathinfo的路由支持。
 *
 * @package skihat.routers
 */
class PathinfoRouterStyle extends AbstractRouterStyle {
    /**
     * @var string 脚本名称。
     */
    public $script = self::DEFAULT_SCRIPT;

    /**
     * @see AbstractRouterStyle::_pathUrl
     */
    protected function _pathUrl(array &$params) {
        # 如果当前包含重写参数，返回脚本名称＋重写参数。
        if ($rewriteUrl = array_key_pop($params, SKIHAT_PARAM_REWRITE)) {
            return $this->script . '/' . $rewriteUrl;
        } else {
            return ($this->script != self::DEFAULT_SCRIPT) ? $this->script : '';
        }
    }
}