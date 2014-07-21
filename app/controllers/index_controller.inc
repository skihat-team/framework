<?php
/**
 * index_controller.inc 默认控制器声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   app.controllers
 * @version   1.0
 *
 * @created   2012-08-07
 * @modified  2013-08-12
 */

/**
 * 示例默认控制器，提供默认服务。
 *
 * @package app.controllers
 */
class IndexController extends ApplicationController {
    public function indexAction() {
        $this['hello'] = 'Hello Skihat PHP MVC';
    }
}