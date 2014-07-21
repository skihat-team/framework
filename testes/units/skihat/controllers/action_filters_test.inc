<?php

/**
 * action_filters_test.inc 提供活动过滤器集合的单元支持。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.controllers
 * @version   1.0
 *
 * @created   2012-07-22
 * @modified  2013-08-12
 */

# 引用测试启动文件。
require_once(dirname(dirname(dirname(__FILE__))) . '/startup.inc');

# 引用被测试类.
Skihat::import('controllers.controller', SKIHAT_PATH_LIBRARY);

/**
 * 活动过滤管理器，单元测试服务类。
 *
 * @package skihat.controllers
 */
class ActionFiltersTest extends PHPUnit_Framework_TestCase {
    /**
     * 测试初始化和完成。
     */
    public function testInitialize() {
        $filters = $this->_createFilters(array(
            'default' => '#DefaultActionFilter'
        ));

        $defaultFilter = $filters->filter('default');
        $this->assertTrue($defaultFilter->initializeCall);
    }

    /**
     * 测试执行完成操作。
     */
    public function testComplete() {
        $filters = $this->_createFilters(array(
            'default' => '#DefaultActionFilter'
        ));

        $filters->complete();

        $defaultFilter = $filters->filter('default');
        $this->assertTrue($defaultFilter->completeCall);
    }

    /**
     * 测试执行。
     */
    public function testInvoke() {
        $filters = $this->_createFilters(array(
            'default' => '#DefaultActionFilter'
        ));

        $defaultFilter = $filters->filter('default');
        $this->assertTrue($filters->invoke('indexAction'));
        $this->assertTrue($defaultFilter->invokeCall);
    }

    /**
     * 测试执行，返回false值。
     */
    public function testInvokeForFalse() {
        $filters = $this->_createFilters(array(
            'one' => '#DefaultActionFilter',
            'two' => array(SKIHAT_IOC_CLASS => '#DefaultActionFilter', 'invokeReturn' => false),
            'three' => '#DefaultActionFilter'
        ));

        $oneFilter = $filters->filter('one');
        $twoFilter = $filters->filter('two');
        $threeFilter = $filters->filter('three');
        $this->assertFalse($filters->invoke('indexAction'));
        $this->assertTrue($oneFilter->invokeCall);

        $this->assertTrue($twoFilter->invokeCall);
        $this->assertFalse($threeFilter->invokeCall);
    }

    /**
     * 测试渲染回调。
     */
    public function testRender() {
        $filters = $this->_createFilters(array(
            'default' => '#DefaultActionFilter'
        ));

        $defaultFilter = $filters->filter('default');

        $options = array();
        $this->assertTrue($filters->render($options));
        $this->assertTrue($defaultFilter->renderCall);
    }

    /**
     * 测试渲染回调，返回false。
     */
    public function testRenderForFalse() {
        $filters = $this->_createFilters(array(
            'one' => '#DefaultActionFilter',
            'two' => array(SKIHAT_IOC_CLASS => '#DefaultActionFilter', 'renderReturn' => false),
            'three' => '#DefaultActionFilter'
        ));

        $oneFilter = $filters->filter('one');
        $twoFilter = $filters->filter('two');
        $threeFilter = $filters->filter('three');

        $options = array();

        $this->assertFalse($filters->render($options));
        $this->assertTrue($oneFilter->renderCall);
        $this->assertTrue($twoFilter->renderCall);
        $this->assertFalse($threeFilter->renderCall);
    }

    /**
     * 根据$filters创建活动过滤集实例。
     *
     * @param array $filters IActionFilter实例Ioc配置数组。
     *
     * @return ActionFilters
     */
    protected function _createFilters(array $filters) {
        return new ActionFilters(new ActionFilterController($filters));
    }
}

/**
 * 活动过滤器控制器，提供ActionFilters单元测试所需服务支持。
 *
 * @package skihat.controllers
 */
class ActionFilterController extends Controller {
    public function __construct($filters = array()) {
        $this->_actionFilters = $filters;
    }

    public function actionFilters() {
        return $this->_actionFilters;
    }

    protected $_actionFilters = array();
}

/**
 * 默认活动过滤器，提供ActionFilters单元测试所需服务支持。
 *
 * @package skihat.controllers
 */
class DefaultActionFilter implements IActionFilter {
    public $invokeReturn = true;
    public $invokeCall = false;

    public $renderReturn = true;
    public $renderCall = false;

    public $initializeCall = false;
    public $completeCall = false;

    public function initialize(Controller $controller) {
        $this->initializeCall = true;
    }

    public function invoke($actionMethod) {
        $this->invokeCall = true;
        return $this->invokeReturn;
    }

    public function render(array &$options) {
        $this->renderCall = true;
        return $this->renderReturn;
    }

    public function complete() {
        $this->completeCall = true;
    }
}