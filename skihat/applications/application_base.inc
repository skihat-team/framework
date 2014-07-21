<?php
/**
 * application_base.inc 提供MVC前端应用控制器声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.applications
 * @version   1.0
 *
 * @created   2012-07-23
 * @modified  2013-08-09
 */

/**
 * 应用前端基础类，提供应用初始化和控制器分发处理。
 *
 * @package skihat.applications
 */
abstract class ApplicationBase {
    /**
     * @const string 错误控制器，当发生错误时执行错误控制器。
     */
    const ERROR_CONTROLLER = 'controllers.error_controller';

    /**
     * @const string 未找到控制器，当控制器未找到时调用NotFoundController。
     */
    const NOT_FOUND_CONTROLLER = 'controllers.not_found_controller';

    /**
     * 初始化Application类实例。
     */
    public function __construct() {
        # 创建内部实例。
        $this->_initializeMembers();
        $this->_globalHandlers();

        # 绑定当前应用请求实例。
        self::$_instance = $this;
    }

    /**
     * 执行初始化方法。
     */
    public function initialize() {
        $this->_initializeConfig();
        $this->_initializeCaches();
        $this->_initializeRouters();
    }

    /**
     * 执行控制器分发，将请求转换到控制器。
     */
    public function dispatching() {
        # 导入应用控制器。
        Skihat::import('appservs.application_controller', array(
            SKIHAT_PATH_APP, SKIHAT_PATH_LIBRARY
        ));

        $ctlObj = $this->_createController();
        $ctlObj->invoke($this->_request, $this->_response);
    }

    /**
     * 全局完成处理方法，本方法由_globalHandlers自动绑定。
     */
    public function globalComplete() {
        # 如果在执行过程中发生错误。
        if ($error = error_get_last()) {
            $message = $this->_errorMessage($error['type'], $error['message'], $error['file'], $error['line']);
            $this->_invokeError(new SkihatException($message, $error['type']));
        }

        $this->_outputResponse();
        $this->_outputDebugs();
    }

    /**
     * @see set_error_handler
     */
    public function globalError($type, $msg, $file, $line) {
        # 格式化错误消息。
        $message = $this->_errorMessage($type, $msg, $file, $line);

        switch ($type) {
            case E_ERROR:
            case E_PARSE:
            case E_CORE_ERROR:
            case E_COMPILE_ERROR:
            case E_USER_ERROR:
                $this->_invokeError(new SkihatException($message, $type));
                break;
            default:
                error_log($message);
        }
    }

    /**
     * @see set_exception_handler
     */
    public function globalException($ex) {
        $this->_invokeError($ex);
    }

    /**
     * 返回应用前端唯一实例。
     *
     * @return ApplicationBase
     */
    public static function instance() {
        return self::$_instance;
    }

    /**
     * 获取全局唯一请求实例。
     *
     * @return ApplicationRequest
     */
    public static function request() {
        return self::$_instance->_request;
    }

    /**
     * 获取全局唯一响应实例。
     *
     * @return ApplicationResponse
     */
    public static function response() {
        return self::$_instance->response();
    }

    /**
     * 初始化实例成员，请求实例和响应实例。
     */
    protected function _initializeMembers() {
        $this->_request = Skihat::ioc('applications.application_request', SKIHAT_PATH_LIBRARY);
        $this->_response = Skihat::ioc('applications.application_response', SKIHAT_PATH_LIBRARY);;
    }

    /**
     * 绑定全局处理器。
     */
    protected function _globalHandlers() {
        set_error_handler(array($this, 'globalError'));
        set_exception_handler(array($this, 'globalException'));
        register_shutdown_function(array($this, 'globalComplete'));
    }

    /**
     * 初始化路由组件。
     */
    protected function _initializeRouters() {
        # 导入路由和路由配置
        Skihat::import('routers.router', SKIHAT_PATH_LIBRARY);
        Skihat::import('boots.routing', SKIHAT_PATH_APP);

        $params = Router::match($_GET);
        if ($params === false && SKIHAT_DEBUG) {
            throw new RouterException('ApplicationBase::dispatching not match rule.');
        } else if ($params === false) {
            $params = $this->_notFoundParams();
        }

        # 初始化请求
        $this->_request->initialize($params, $_POST, $_FILES);
        $basicParams = $this->_basicParams();

        Router::defaultParams($basicParams, $params);
    }

    /**
     * 初始化配置信息。
     */
    protected function _initializeConfig() {
        Skihat::import('boots.config', SKIHAT_PATH_APP);
    }

    /**
     * 初始缓存组件。
     */
    protected function _initializeCaches() {
        # 如果当前开启缓存，则调用缓存组件。
        if (enable_cached()) {
            Skihat::import('kernels.caches.cache', SKIHAT_PATH_LIBRARY);
        }
    }

    /**
     * 根据应用请求获取控制器实例。
     *
     * @return Controller
     *
     * @throws FileNotFoundException 如果指定控制器文件不存在，将引发异常。
     */
    protected function _createController() {
        $ctlFile = $this->_ctlFile();
        $ctlPath = $this->_ctlPath();

        if (Skihat::locate($ctlFile, $ctlPath)) {
            return Skihat::ioc($ctlFile, $ctlPath);
        } else if (SKIHAT_DEBUG) {
            throw new FileNotFoundException('ApplicationBase::dispatching not found controller file.');
        }

        return Skihat::ioc(self::NOT_FOUND_CONTROLLER, array(SKIHAT_PATH_APP, SKIHAT_PATH_LIBRARY));
    }

    /**
     * 返回控制器文件名称.
     *
     * @return string
     */
    protected function _ctlFile() {
        $request = $this->_request;

        # 获取控制器名称，如果控制器指定时没有正确使用下划线将不能正确查询。
        $fullFile = $request->controller() . '_controller';
        if ($package = $request->package()) {
            $fullFile = ($package . SKIHAT_PACKAGE_SEPARATOR . $fullFile);
        }

        # 指定控制器目录，如果包含模块，则指定模块信息。
        $fullFile = ('controllers' . SKIHAT_PACKAGE_SEPARATOR . $fullFile);
        if ($module = $request->module()) {
            $fullFile = ($module . SKIHAT_PACKAGE_SEPARATOR . $fullFile);
        }

        return $fullFile;
    }

    /**
     * 返回控制器所在路径信息。
     *
     * @return string
     */
    protected function _ctlPath() {
        return $this->_request->module() ? SKIHAT_PATH_APP_MODULES : array(SKIHAT_PATH_APP, SKIHAT_PATH_LIBRARY);
    }

    /**
     * 执行错误处理，将错误记录到错误日志中，并调用相应的处理控制器。
     *
     * @param Exception $ex 异常实例。
     */
    protected function _invokeError($ex) {
        # 导入全局控制器文件，并记录错误处理信息。
        Skihat::import('appservs.application_controller', array(SKIHAT_PATH_APP, SKIHAT_PATH_LIBRARY));
        error_log($ex->getTraceAsString());

        # 实例化错误处理控制器，并且设置异常参数。
        $ctlObj = Skihat::ioc(self::ERROR_CONTROLLER, array(SKIHAT_PATH_APP, SKIHAT_PATH_LIBRARY));
        $ctlObj->ex = $ex;

        $ctlObj->invoke($this->_request, $this->_response);
    }

    /**
     * 根据$type，$message，$file和$line生成错误格式化消息。
     *
     * @param string $type    错误类型。
     * @param string $message 错误消息。
     * @param string $file    错误发生的文件。
     * @param string $line    错误发生的地址。
     *
     * @return string
     */
    protected function _errorMessage($type, $message, $file, $line) {
        return sprintf('[%d]%s #%s(%d)', $type, $message, $file, $line);
    }

    /**
     * 根据当前请求信息返回基本参数数组。
     *
     * @return array
     */
    protected function _basicParams() {
        # 初始化路中默认参数值
        $basicParams = array(
            SKIHAT_PARAM_CONTROLLER => $this->_request->controller(),
            SKIHAT_PARAM_ACTION => $this->_request->action()
        );

        if ($package = $this->_request->package()) {
            $basicParams[SKIHAT_PARAM_PACKAGE] = $package;
        }

        if ($module = $this->_request->module()) {
            $basicParams[SKIHAT_PARAM_MODULE] = $module;
        }

        return $basicParams;
    }

    /**
     * 输出响应信息。
     */
    protected function _outputResponse() {
        $this->_response->render();
    }

    /**
     * 测试调试信息。
     */
    protected function _outputDebugs() {
        # 如果当前为调度状态，将所有的调试信息写入到调试日志中。
        if (SKIHAT_DEBUG) {
            Skihat::debugLines(false, 'debugs.log');
        }
    }

    /**
     * @return array 返回参数为未找到参数。
     */
    protected function _notFoundParams() {
        return array(
            SKIHAT_PARAM_CONTROLLER => 'not_found',
            SKIHAT_PARAM_ACTION => SKIHAT_PARAM_DEFAULT_ACTION,
            SKIHAT_PARAM_PACKAGE => SKIHAT_PARAM_DEFAULT_PACKAGE,
            SKIHAT_PARAM_MODULE => SKIHAT_PARAM_DEFAULT_MODULE
        );
    }

    /**
     * @var ApplicationRequest 应用请求实例。
     */
    protected $_request = null;

    /**
     * @var ApplicationResponse 应用响应对象。
     */
    protected $_response = null;

    /**
     * @var ApplicationBase 应用请求唯一实例。
     */
    protected static $_instance = null;
}