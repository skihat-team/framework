<?php
/**
 * application_controller.inc 提供应用控制器声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.appservs
 * @version   1.0
 *
 * @created   2012-07-23
 * @modified  2013-08-13
 */

# 引用控制器基础类。
Skihat::import('controllers.controller', SKIHAT_PATH_LIBRARY);

/**
 * 应用控制器，提供应用中所有控制器的父类声明。
 *
 * @package skihat.appservs
 */
class ApplicationController extends Controller {

}