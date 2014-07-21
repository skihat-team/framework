<?php
/**
 * skihat.inc 提供Skihat核心类的声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.routers
 * @version   1.0
 *
 * @created   2012-06-12
 * @modified  2013-08-02
 */

# 引用测试启动文件。
require_once(dirname(dirname(dirname(__FILE__))) . '/startup.inc');

# 引用被测试类.
Skihat::import('routers.router', SKIHAT_PATH_LIBRARY);

/**
 * 路由样式测试类，提供路由样式的单元测试服务。
 *
 * @package skihat.routers
 */
class RouterStylesTest extends PHPUnit_Framework_TestCase {
    /**
     * 测试普通路由样式，参数为空。
     */
    public function testNormalRouterStyle() {
        $this->assertEquals('/', $this->_createNormalURL(array()));
    }

    /**
     * 测试普通路由样式，包含参数值。
     */
    public function testNormalRouterStyleForParams() {
        $url = $this->_createNormalURL(array('ctl' => 'index', 'act' => '&?<>'));
        $this->assertEquals('/?ctl=index&act=%26%3F%3C%3E', $url);
    }

    /**
     * 测试普通样式，指定锚信息。
     */
    public function testNormalRouterStyleForAnchor() {
        $url = $this->_createNormalURL(array(SKIHAT_PARAM_ANCHOR => 'top'));
        $this->assertEquals('/#top', $url);
    }

    /**
     * 测试普通样式，指定参数和锚信息。
     */
    public function testNormalRouterStyleForAnchorAndParams() {
        $url = $this->_createNormalUrl(array(SKIHAT_PARAM_ANCHOR => 'top', 'act' => 'edit', 'ctl' => 'index'));
        $this->assertEquals('/?act=edit&ctl=index#top', $url);
    }

    /**
     * 测试普通样式，指定域名。
     */
    public function testNormalRouterStyleForDomain() {
        $url = $this->_createNormalURL(array(), 'http://www.example.com');
        $this->assertEquals('http://www.example.com/', $url);
    }

    /**
     * 测试普通样式，指定域名信息。
     */
    public function testNormalRouterStyleForDomainAndParams() {
        $url = $this->_createNormalURL(array('act' => 'edit', 'ctl' => 'idx'), 'http://www.example.com');
        $this->assertEquals('http://www.example.com/?act=edit&ctl=idx', $url);
    }

    /**
     * 测试普通样式，指定域名和锚。
     */
    public function testNormalRouterStyleForDomainAndAnchor() {
        $url = $this->_createNormalURL(array(SKIHAT_PARAM_ANCHOR => 'top'), 'http://www.example.com');
        $this->assertEquals('http://www.example.com/#top', $url);
    }

    /**
     * 测试普通样式，指定域名，参数和锚。
     */
    public function testNormalRouterStyleForDomainAndAnchorAndParams() {
        $url = $this->_createNormalURL(array('action' => 'edit', SKIHAT_PARAM_ANCHOR => 'top'), 'http://www.example.com');
        $this->assertEquals('http://www.example.com/?action=edit#top', $url);
    }

    /**
     * 测试普通样式，指定脚名称。
     */
    public function testNormalRouterStyleForScript() {
        $url = $this->_createNormalURL(array(), false, 'admin.php');
        $this->assertEquals('/admin.php', $url);
    }

    /**
     * 测试普通样式，指定脚本和参数信息。
     */
    public function testNormalRouterStyleForScriptAndParams() {
        $url = $this->_createNormalURL(array('action' => 'edit'), false, 'admin.php');
        $this->assertEquals('/admin.php?action=edit', $url);
    }

    /**
     * 测试普通样式，指定脚本和锚信息。
     */
    public function testNormalRouterStyleForScriptAndAnchor() {
        $url = $this->_createNormalURL(array(SKIHAT_PARAM_ANCHOR => 'top'), false, 'admin.php');
        $this->assertEquals('/admin.php#top', $url);
    }

    /**
     * 测试普通样式，指定脚本和域名。
     */
    public function testNormalRouterStyleForScriptAndDomain() {
        $url = $this->_createNormalURL(array(), 'http://www.example.com', 'admin.php');
        $this->assertEquals('http://www.example.com/admin.php', $url);
    }

    /**
     * 测试普通样式，指定脚本，域名参数和锚。
     */
    public function testNormalRouterStyleForScriptAndParamsAndAnchorAndDomain() {
        $url = $this->_createNormalURL(array('act' => 'edit', SKIHAT_PARAM_ANCHOR => 'top'), 'http://www.example.com', 'admin.php');
        $this->assertEquals('http://www.example.com/admin.php?act=edit#top', $url);
    }

    /**
     * 测试重写路由样式，参数为空。
     */
    public function testRewriteRouterStyle() {
        $url = $this->_createRewriteURL(array());
        $this->assertEquals('/', $url);
    }

    /**
     * 测试重写路由样式。
     */
    public function testRewriteRouterStyleForRewrite() {
        $url = $this->_createRewriteURL(array(SKIHAT_PARAM_REWRITE => 'index/edit'));
        $this->assertEquals('/index/edit', $url);
    }

    /**
     * 测试重写路由样式，指定更多参数。
     */
    public function testRewriteRouterStyleForRewriteAndParams() {
        $url = $this->_createRewriteURL(array(SKIHAT_PARAM_REWRITE => 'index/edit', 'id' => '&?<>'));
        $this->assertEquals('/index/edit?id=%26%3F%3C%3E', $url);
    }

    /**
     * 测试重写路由组件，指定锚。
     */
    public function testRewriteRouterStyleForAnchor() {
        $url = $this->_createRewriteURL(array(SKIHAT_PARAM_ANCHOR => 'top'));
        $this->assertEquals('/#top', $url);
    }

    /**
     * 测试重写路由样式，指定锚和重写参数。
     */
    public function testRewriteRouterStyleForAnchorAndRewrite() {
        $url = $this->_createRewriteURL(array(SKIHAT_PARAM_ANCHOR => 'top', SKIHAT_PARAM_REWRITE => 'index/edit'));
        $this->assertEquals('/index/edit#top', $url);
    }

    /**
     * 测试重写路由样式，指定域名信息。
     */
    public function testRewriteRouterStyleForDomain() {
        $url = $this->_createRewriteURL(array(), 'http://www.example.com');
        $this->assertEquals('http://www.example.com/', $url);
    }

    /**
     * 测试重写路由样式，指定域名和重写参数。
     */
    public function testRewriteRouterStyleForDomainAndRewrite() {
        $url = $this->_createRewriteURL(array(SKIHAT_PARAM_REWRITE => 'index/edit'), 'http://www.example.com');
        $this->assertEquals('http://www.example.com/index/edit', $url);
    }

    /**
     * 测试重写路由样式，指定域名，重写参数，锚和参数。
     */
    public function testRewriteRouterStyleForDomainAndRewriteAndAnchorAndParams() {
        $url = $this->_createRewriteURL(array(SKIHAT_PARAM_REWRITE => 'index/edit', SKIHAT_PARAM_ANCHOR => 'top', 'id' => 12), 'http://www.example.com');
        $this->assertEquals('http://www.example.com/index/edit?id=12#top', $url);
    }

    /**
     * 测试pathinfo路由样式。
     */
    public function testPathinfoRouterStyle() {
        $url = $this->_createPathinfoURL(array());
        $this->assertEquals('/', $url);
    }

    /**
     * 测试pathinfo路由样式，指定重写参数。
     */
    public function testPathinfoRouterStyleForRewrite() {
        $url = $this->_createPathinfoURL(array(SKIHAT_PARAM_REWRITE => 'index/edit'));
        $this->assertEquals('/index.php/index/edit', $url);
    }

    /**
     * 测试pathinfo路由样式，指定重写和参数。
     */
    public function testPathinfoRouterStyleForRewriteAndParams() {
        $url = $this->_createPathinfoURL(array(SKIHAT_PARAM_REWRITE => 'index/edit', 'id' => '&?<>'));
        $this->assertEquals('/index.php/index/edit?id=%26%3F%3C%3E', $url);
    }

    /**
     * 测试pathinfo路由样式，指定参数。
     */
    public function testPathinfoRouterStyleForParams() {
        $url = $this->_createPathinfoURL(array('act' => 'edit'));
        $this->assertEquals('/?act=edit', $url);
    }

    /**
     * 测试pathinfo路由样式，指定锚。
     */
    public function testPathinfoRouterStyleForAnchor() {
        $url = $this->_createPathinfoURL(array(SKIHAT_PARAM_ANCHOR => 'top'));
        $this->assertEquals('/#top', $url);
    }

    /**
     * 测试pathinfo路由样式，指定锚和重写值。
     */
    public function testPathinfoRouterStyleForAnchorAndRewrite() {
        $url = $this->_createPathinfoURL(array(SKIHAT_PARAM_ANCHOR => 'top', SKIHAT_PARAM_REWRITE => 'index/edit'));
        $this->assertEquals('/index.php/index/edit#top', $url);
    }

    /**
     * 测试pathinfo路由样式，指定域名。
     */
    public function testPathinfoRouteStyleForDomain() {
        $url = $this->_createPathinfoURL(array(), 'http://www.example.com');
        $this->assertEquals('http://www.example.com/', $url);
    }

    /**
     * 测试pathinfo路则样式，指定重写参数。
     */
    public function testPathinfoRouterStyleForDomainAndRewriteAndParams() {
        $url = $this->_createPathinfoURL(array(SKIHAT_PARAM_REWRITE => 'index/edit', 'id' => 12), 'http://www.example.com');
        $this->assertEquals('http://www.example.com/index.php/index/edit?id=12', $url);
    }

    /**
     * 测试pathinfo路由样式，指定脚本参数。
     */
    public function testPathinfoRouterStyleForScript() {
        $url = $this->_createPathinfoURL(array('id' => 12), 'http://www.example.com', 'admin.php');
        $this->assertEquals('http://www.example.com/admin.php?id=12', $url);
    }

    /**
     * 根据$params，$domain和$script，创建默认样式URL地址信息。
     *
     * @param array       $params URL参数集。
     * @param bool|string $domain 域名。
     * @param bool|string $script 脚本名称。
     *
     * @return mixed|string
     */
    protected function _createNormalURL(array $params, $domain = false, $script = false) {
        $style = new NormalRouterStyle();
        if ($domain) $style->domain = $domain;
        if ($script) $style->script = $script;

        return $style->fullUrl($params);
    }

    /**
     * 根据$params和$domain创建重写URL地址信息。
     *
     * @param array $params URL参数集。
     * @param bool  $domain 域名。
     *
     * @return string
     */
    protected function _createRewriteURL(array $params, $domain = false) {
        $style = new RewriteRouterStyle();
        $style->domain = $domain;

        return $style->fullUrl($params);
    }

    /**
     * 根据$params，$domain和$script返回pathinfo样式URL地址。
     *
     * @param array $params 参数集合。
     * @param bool  $domain 域名。
     * @param bool  $script 脚本信息。
     *
     * @return string
     */
    protected function _createPathinfoURL(array $params, $domain = false, $script = false) {
        $style = new PathinfoRouterStyle();
        if ($domain) $style->domain = $domain;
        if ($script) $style->script = $script;

        return $style->fullUrl($params);
    }
}