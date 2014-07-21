<?php
/**
 * application_request_test.inc 提供应用请求单元测试类声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.applications
 * @version   1.0
 *
 * @created   2012-07-23
 * @modified  2013-08-11
 */

# 引用测试启动文件。
require_once(dirname(dirname(dirname(__FILE__))) . '/startup.inc');

# 引用被测试类。
Skihat::import('applications.application_base', SKIHAT_PATH_LIBRARY);

/**
 * 应用前端测试类，提供应用前端类的单元测试服务。
 *
 * @package skihat.applications
 */
class ApplicationBaseTest extends PHPUnit_Framework_TestCase {

    /**
     * 测试控制器分发。
     */
    public function testDispatching() {
        $this->_createApplication(array(SKIHAT_PARAM_CONTROLLER => 'default'));

        $this->assertTrue(DefaultController::$callInvoke);
    }

    /**
     * 测试控制器分发，包含包信息。
     */
    public function testDispatchingForPackage() {
        $this->_createApplication(array(SKIHAT_PARAM_CONTROLLER => 'admin', SKIHAT_PARAM_PACKAGE => 'admins'));

        $this->assertTrue(AdminController::$callInvoke);
    }

    /**
     * 测试控制器分发，指定模块信息。
     */
    public function testDispatchingForModule() {
        $this->_createApplication(array(SKIHAT_PARAM_CONTROLLER => 'archive', SKIHAT_PARAM_MODULE => 'archives'));

        $this->assertTrue(ArchiveController::$callInvoke);
    }

    /**
     * 测试控制器分发，指定模块包信息。
     */
    public function testDispatchingForModulePackage() {
        $this->_createApplication(array(
            SKIHAT_PARAM_CONTROLLER => 'admin_archive',
            SKIHAT_PARAM_MODULE => 'archives',
            SKIHAT_PARAM_PACKAGE => 'admins'));

        $this->assertTrue(AdminArchiveController::$callInvoke);
    }

    /**
     * 测试控制器分发，文件没有找到。
     *
     * @expectedException FileNotFoundException
     */
    public function testDispatchingForFileNotFound() {
        $this->_createApplication(array(SKIHAT_PARAM_CONTROLLER => 'index'));
    }

    /**
     * 测试控制器分发，文件没有找到。
     *
     * @expectedException TypeNotFoundException
     */
    public function testDispatchingForClassNotFound() {
        $this->_createApplication(array(SKIHAT_PARAM_CONTROLLER => 'miscls'));
    }

    /**
     * 创建应用测试所需实例。
     */
    protected function _createApplication(array $params, array $forms = array()) {
        $app = new DefaultApplication($params, $forms);
        $app->initialize();
        $app->dispatching();

        return $app;
    }
}

/**
 * 默认应用，提供应用测试所需的服务支持。
 *
 * @package skihat.applications
 */
class DefaultApplication extends ApplicationBase {
    public function __construct(array $params = array(), $forms = array()) {
        parent::__construct();

        $this->_params = $params;
        $this->_forms = $forms;
    }

    public function initialize() {
        $this->_initializeRouters();
    }

    protected function _globalHandlers() {

    }

    protected function _initializeRouters() {
        $this->_request->initialize($this->_params, $this->_forms);
    }


    protected $_params = array();
    protected $_forms = array();
}
