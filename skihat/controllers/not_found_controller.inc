<?php

/**
 * not_found_controller.inc 声明未找到控制器。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.controllers
 * @version   1.0
 *
 * @created   2012-07-23
 * @modified  2013-08-13
 */

/**
 * 未找到控制器，当前端应当发控制器未找到时调用当前控制器。
 *
 * @package skihat.controllers
 */
class NotFoundController extends ApplicationController {
    /**
     * @see Controller::invoke
     */
    public function invoke(ApplicationRequest $request, ApplicationResponse $response) {
        $this->_initialize($request, $response);
        $this->_initializeFilters();
        $this->error(404);

        $this->_completeFilters();
    }
}
