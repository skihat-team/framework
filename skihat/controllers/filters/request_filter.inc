<?php
/**
 * request_filter.inc 提供请求过滤器声明。
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

/**
 * 请求过滤器，提供为非html格式自动增加pass参数选项。
 *
 * @package skihat.controllers.filters
 */
class RequestFilter implements IActionFilter {
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
        return true;
    }

    /**
     * @see IActionFilter::render
     */
    public function render(array &$options) {
        # 获取请求格式和请求参数。
        $format = $this->_controller->request->fmt();
        $method = client_method();
        
        # 如果请求格式不为默认值或者非get请求。
        if ($format != SKIHAT_PARAM_DEFAULT_FMT || $method != 'get') {
            $pass = array();
            # 如果请求方法为非get方法
            if ($format != SKIHAT_PARAM_DEFAULT_FMT) array_push($pass, $fmt);
            if ($method != 'get') array_push($pass, client_method());

            if (!isset($options['pass'])) $options['pass'] = $pass;
            else $options['pass'] = array_merge($pass, (array)$options['pass']);
        }

        return TRUE;
    }

    /**
     * @see IActionFilter::complete
     */
    public function complete() {
    }

    /**
     * @var Controller 控制器。
     */
    protected $_controller = null;
}