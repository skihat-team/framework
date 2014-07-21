<?php
/**
 * requery.inc 提供请求查询助手，根据请求生成参数。
 *
 * @copyright   2012-2013 Skihat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      Skihat Team
 *
 * @package     skihat.views._helpers
 * @version     1.0
 *
 * @create      2012-10-16
 * @modify      2013-09-13
 */

/**
 * Requery 请求查询类，返回请求的查询结果。
 *
 * @package skihat.views._helpers
 */
class Requery {
    /**
     * 使用$request初始化Requery实例。
     *
     * @param ApplicationRequest $request 应用请求实例。
     */
    public function __construct(ApplicationRequest $request) {
        $this->_request = $request;
    }

    /**
     * 根据$module返回一个布尔值，判断是否为$module模块。
     *
     * @param string $module 模块名称。
     *
     * @return bool
     */
    public function is_m($module) {
        return $this->_request->module() == $module;
    }

    /**
     * 根据$module、$package参数，判断当前请求是否为指定模块和包。
     *
     * @param string $module  模块名称。
     * @param string $package 包名称。
     *
     * @return bool
     */
    public function is_mp($module, $package) {
        return $this->_request->module() == $module &&
        $this->_request->package() == $package;
    }

    /**
     * 根据$module、$package和$controller参数，判断当前请求是否为指定模块，包和控制器。
     *
     * @param string $module     模块名称。
     * @param string $package    包名称。
     * @param string $controller 控制器名称。
     *
     * @return bool
     */
    public function is_mpc($module, $package, $controller) {
        return $this->_request->module() == $module &&
        $this->_request->package() == $package &&
        $this->_request->controller() == $controller;
    }

    /**
     * 根据$module、$package、$controller和$action参数，是否为指定模块、包、控制器和活动。
     *
     * @param string $module     模块名称。
     * @param string $package    包名称。
     * @param string $controller 控制器名称。
     * @param string $action     活动名称。
     *
     * @return bool
     */
    public function is_mpca($module, $package, $controller, $action) {
        return $this->_request->module() == $module &&
        $this->_request->package() == $package &&
        $this->_request->controller() == $controller &&
        $this->_request->action() == $action;
    }

    /**
     * 根据$module和$controller判断当前请求是否为指定模块和控制器。
     *
     * @param string $module     模块名称。
     * @param string $controller 控制器名称。
     *
     * @return bool
     */
    public function is_mc($module, $controller) {
        return $this->_request->module() == $module &&
        $this->_request->controller() == $controller;
    }

    /**
     * 根据$controller返回一个布尔值，判断是否为指定控制器。
     *
     * @param string $controller 控制器名称。
     *
     * @return bool
     */
    public function is_c($controller) {
        return $this->_request->controller() == $controller;
    }

    /**
     * 根据$controller和$action返回一个布尔值，不判断模块和包。
     *
     * @param string $controller 控制器名称。
     * @param string $action     活动名称。
     *
     * @return bool
     */
    public function is_ca($controller, $action) {
        return $this->_request->controller() == $controller &&
        $this->_request->action() == $action;
    }

    /**
     * 根据$controller和$action返回一个布尔值，不包模块和包。
     *
     * @param string $controller 控制器名称。
     * @param string $action     活动名称。
     *
     * @return bool
     */
    public function is_caf($controller, $action) {
        return $this->_request->controller() == $controller &&
        $this->_request->action() == $action &&
        $this->_request->module() == SKIHAT_PARAM_DEFAULT_MODULE &&
        $this->_request->package() == SKIHAT_PARAM_DEFAULT_PACKAGE;
    }

    /**
     * 根据$action返回一个布求值，判断是否为指定活动。
     *
     * @param string $action 活动名称。
     *
     * @return bool
     */
    public function is_a($action) {
        return $this->_request->action() == $action;
    }

    /**
     * 根据$param和$value返回一个布尔值，判断当前$param请求是否为$value的值。
     *
     * @param string $param 参数名称。
     * @param mixed  $value 请求值。
     *
     * @return bool
     */
    public function is_pv($param, $value) {
        return $this->_request->query($param) == $value;
    }
}