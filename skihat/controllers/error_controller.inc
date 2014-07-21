<?php
/**
 * error_controller.inc 声明错误异常控制器。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.controllers
 * @version   1.0
 *
 * @created   2012-07-23
 * @modified  2013-08-12
 */

/**
 * 错误控制器，当发生错误时前端控制器会自动将错误交给当前控制器进行处理。
 *
 * @package skihat.controllers
 */
class ErrorController extends ApplicationController {
    /**
     * @var Exception 错误异常声明。
     */
    public $ex = null;

    /**
     * @see Controller::invoke
     */
    public function invoke(ApplicationRequest $request, ApplicationResponse $response) {
        $this->_initialize($request, $response);
        $this->_initializeFilters();
        $this->_invokeError();
        $this->_completeFilters();
    }

    /**
     * 执行错误处理操作。
     */
    protected function _invokeError() {
        $this['ex'] = $this->ex;
        $this->error(500);
    }
}
