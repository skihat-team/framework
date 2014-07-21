<?php
/**
 * controller.inc 提供控制器类声明。
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

/**
 * 活动过滤器，提供控制器的额外服务。
 *
 * @package skihat.controllers
 */
interface IActionFilter {
    /**
     * 使用$controller初始化IActionFilter接口实例。
     *
     * @param Controller $controller 控制器。
     */
    function initialize(Controller $controller);

    /**
     * 使用$actionMethod参数，执行活动处理前回调，如果返回值为true表示继续执行，false表示中断执行。
     *
     * @param string $actionMethod 活动方法名称。
     *
     * @return bool
     */
    function invoke($actionMethod);

    /**
     * 使用$options参数，执行模板渲染前回调，如果返回true表示继续执行，false表示中断执行。
     *
     * @param array $options 渲染选项参数。
     *
     * @return bool
     */
    function render(array &$options);

    /**
     * 控制器执行完成方法，当控制器执行完成后将回调本方法。
     */
    function complete();
}

/**
 * 活动过滤集，提供对活动过滤器的管理和支持。
 *
 * @package skihat.controllers
 */
class ActionFilters {
    /**
     * 使用$controller初始化活动过滤器集合。
     *
     * @param Controller $controller 控制器。
     */
    public function __construct(Controller $controller) {
        $this->_controller = $controller;
        $this->_initializeFilters();
    }

    /**
     * 使用$actionMethod执行过滤器回调，并返回执行结果。
     *
     * @param string $actionMethod 活动方法名称。
     *
     * @return bool
     */
    public function invoke($actionMethod) {
        # 从过滤器集合中获取过滤器，并执行方法。
        foreach ($this->_actionFilters as $filter) {
            if (false === $filter->invoke($actionMethod)) {
                return false;
            }
        }

        return true;
    }

    /**
     * 使用$options执行模板渲染前回调，并返回执行结果。
     *
     * @param array $options 渲染选项。
     *
     * @return bool
     */
    public function render(array &$options) {
        # 从过滤器集合中，获取过滤器，并执行渲染方法。
        foreach ($this->_actionFilters as $filter) {
            if (false === $filter->render($options)) {
                return false;
            }
        }

        return true;
    }

    /**
     * 活动执行完成后回调用方法。
     */
    public function complete() {
        # 从过滤器集合中获取过滤器，并执行完成方法。
        foreach ($this->_actionFilters as $filter) {
            $filter->complete();
        }
    }

    /**
     * 根据$name返回活动过滤器。
     *
     * @param string $name 过滤器名称。
     *
     * @return IActionFilter
     */
    public function filter($name) {
        return $this->_actionFilters[$name];
    }

    /**
     * 执行活动过滤器初始化。
     */
    protected function _initializeFilters() {
        # 获取全部活动过滤器集合。
        $controller = $this->_controller;
        $filters = $controller->actionFilters();

        # 实例化活动过滤器，并执行初始化。
        foreach ($filters as $name => $filter) {
            $actionFilter = Skihat::ioc($filter);
            $actionFilter->initialize($this->_controller);

            $this->_actionFilters[$name] = $actionFilter;
        }
    }

    /**
     * @var Controller 活动相关控制器。
     */
    protected $_controller = null;

    /**
     * @var array 全部活动过滤器。
     */
    protected $_actionFilters = array();
}

/**
 * 控制器组件，提供外部访问的响应处理。
 *
 * @package skihat.controllers
 */
abstract class Controller extends CollectionBase {
    /**
     * @const string 默认视图处理类。
     */
    const DEFAULT_VIEW_CLASS = 'views.theme';

    /**
     * @const string 默认错误处理模板。
     */
    const DEFAULT_ERROR_TEMPLATE = 'error';

    /**
     * @var ApplicationRequest 应用请求对象，默认为null值。
     */
    public $request = null;

    /**
     * @var ApplicationResponse 应用响应对象，默认为null值。
     */
    public $response = null;

    /**
     * @var bool 自动视图渲染变量，如果为true表示活动方法完成后自动调用render方法。
     */
    public $autoRender = true;

    /**
     * 根据$name和$default获取查询字符串的值，如果$name指定值不存在则返回$default值。
     *
     * @param string $name    查询字段名称。
     * @param mixed  $default 默认值，默认为false。
     *
     * @return mixed
     */
    public function query($name, $default = false) {
        return $this->request->query($name, $default);
    }

    /**
     * 根据$name和$default获取提交表单的值，如果$name指定值不存在则返回$default值。
     *
     * @param string $name    表单字段名称。
     * @param mixed  $default 默认值，默认为false。
     *
     * @return mixed
     */
    public function form($name, $default = false) {
        return $this->request->form($name, $default);
    }

    /**
     * 根据$name获取提交文件字段值，如果指定$name字段不存在则返回false。
     *
     * @param string $name 文件字段名称。
     *
     * @return array|bool
     */
    public function file($name) {
        return $this->request->file($name);
    }

    /**
     * 返回一个布尔值，表示当前请求是否为GET方法。
     */
    public function isGet() {
        return client_method() == 'get';
    }

    /**
     * 返回一个布尔值，表示当前请求是否为POST方法。
     */
    public function isPost() {
        return client_method() != 'get';
    }

    /**
     * 使用$request和$response参数，执行控制器。
     *
     * @param ApplicationRequest  $request  应用请求参数。
     * @param ApplicationResponse $response 应用响应参数。
     */
    public function invoke(ApplicationRequest $request, ApplicationResponse $response) {
        $this->_initialize($request, $response);
        $this->_initializeFilters();

        $this->_invokeAction();
        $this->_completeFilters();
    }

    /**
     * 根据$options渲染控制器的执行结果，如果$options为字符串表示指定活动选项。
     *
     * @param array $options 渲染选项。
     */
    public function render($options = array()) {
        # 关闭自动渲染
        $this->autoRender = false;

        # 如果选项为字符串，表示指定活动参数。
        if (is_string($options)) {
            $options = array(SKIHAT_PARAM_ACTION => $options);
        }

        # 执行过滤器渲染回调方法。
        if ($this->_renderFilters($options)) {
            $this->_invokeRender($options);
        }
    }

    /**
     * 根据$title，$sign，$actions和$message设置视图响应消息。
     *
     * @param string $title   响应标题。
     * @param string $sign    响应标志。
     * @param array  $actions 相关活动。
     * @param string $message 完整通知消息。
     */
    public function message($title, $sign = SKIHAT_MESSAGE_INFO, $actions = array(), $message = '') {
        $this->_message = array('title' => $title, 'sign' => $sign, 'actions' => $actions, 'message' => $message);
    }

    /**
     * 根据$url和$delay参数进行客户端跳转响应，如果$url参数为字符串表示跳转到当前控制器的指定活动。
     *
     * @param array|string $url   客户端URL地址，如果是数组将使用路由生成URL地址。
     * @param bool         $full  是否使用完整URL匹配。
     * @param int          $delay 延迟时间（秒）。
     */
    public function redirect($url, $full = false, $delay = 0) {
        if (is_array($url)) {
            $url[Router::FULL_URL] = $full;
            $url = Router::url($url);
        } else if (strpos($url, 'http://') !== 0 && strpos($url, 'https://') !== 0 && $url[0] !== '/') {
            $url = Router::url(array(SKIHAT_PARAM_ACTION => $url, Router::FULL_URL => $full));
        }

        $this['url'] = $url;
        $this['delay'] = $delay;

        $this->_ctlTpl('redirect');
    }

    /**
     * 根据$text提示响应字符串，响应字符串格式数据。
     *
     * @param string $text 文本消息。
     */
    public function text($text) {
        $this['text'] = $text;
        $this->_ctlTpl('text');
    }

    /**
     * 根据$json作为响应信息，响应json格式数据。
     *
     * @param mixed $json 响应格式内容。
     */
    public function json($json) {
        $this['json'] = $json;
        $this->_ctlTpl('json');
    }

    /**
     * 根据$jsonp和$contentType响应跨名请求信息。
     *
     * @param mixed  $jsonp       响应内容。
     * @param string $contentType 响应内容类型。
     */
    public function jsonp($jsonp, $contentType = 'json/application') {
        $this['jsonp'] = $jsonp;
        $this['contentType'] = $contentType;

        $this->_ctlTpl('jsonp');
    }

    /**
     * 根据$errorCode提示错误信息。
     *
     * @param string|int $errorCode 错误代码。
     */
    public function error($errorCode) {
        $this->render(array(
            SKIHAT_PARAM_MODULE => false,
            SKIHAT_PARAM_PACKAGE => false,
            'template' => array('__errors.' . $errorCode, self::DEFAULT_ERROR_TEMPLATE)
        ));
    }

    /**
     * 返回当前控制器，活动过滤器集合（IOC配置数组，默认为空）。
     *
     * @return array
     */
    public function actionFilters() {
        return array();
    }

    /**
     * 导入运行所需的全局模型信息。
     */
    public function actionGlobalModels() {
        Skihat::import('appservs.application_model', array(SKIHAT_PATH_APP, SKIHAT_PATH_LIBRARY));
    }

    /**
     * 导入活动模型信息。
     */
    public function actionModels() {
    }

    /**
     * 返回一个Ioc配置，提供视图类型配置信息（默认为Theme)。
     */
    public function actionView() {
        return array(
            SKIHAT_IOC_CLASS => self::DEFAULT_VIEW_CLASS,
            SKIHAT_IOC_PATHS => SKIHAT_PATH_LIBRARY
        );
    }

    /**
     * 活动空白方法，如果指定的话动名称没有找到将调用本方法，默认提示404错误。
     */
    public function actionBlank() {
        # 如果当前为调试模式，提示错误异常。
        if (SKIHAT_DEBUG) {
            throw new SkihatException('Controller:actionBlank');
        }

        $this->error(404);
    }

    /**
     * 使用$request和$response初化当前控制器。
     *
     * @param ApplicationRequest  $request  应用请求实例。
     * @param ApplicationResponse $response 应用响应实例。
     */
    protected function _initialize(ApplicationRequest $request, ApplicationResponse $response) {
        $this->request = $request;
        $this->response = $response;
    }

    /**
     * 执行活动方法。
     */
    protected function _invokeAction() {
        # 获取活动方法。
        $actionMethod = $this->_actionMethod();

        # 如果执行过滤器成功，导入活动执行模型。
        if ($this->_invokeFilters($actionMethod)) {
            $this->actionGlobalModels();
            $this->actionModels();

            # 如果指定活动方法，在控制器中包含声明。
            if (method_exists($this, $actionMethod)) {
                $this->$actionMethod(); # 执行方法。
            } else {
                $this->actionBlank(); # 执行空白方法。
            }

            if ($this->autoRender) {
                $this->render();
            }
        }
    }

    /**
     * 获取当前请求的活动方法名称。
     *
     * @return string
     */
    protected function _actionMethod() {
        return camel_upper($this->request->action()) . 'Action';
    }

    /**
     * 根据$options执行具体的渲染方法。
     *
     * @param array $options 渲染选项。
     */
    protected function _invokeRender(array $options) {
        # 导入视图接口。
        Skihat::import('views.view', SKIHAT_PATH_LIBRARY);

        # 创建并初始化视图对象。
        $view = Skihat::ioc($this->actionView());
        $view->initialize($this, $options);
        $view->assignAll($this->_vars);
        $view->message($this->_message);

        $view->inflate();
    }

    /**
     * 使用成员变量方法，提供对过滤器的便捷访问，如果指定$name过滤器不存在将发生索引错误。
     *
     * @param string $name 过滤器名称。
     *
     * @return IActionFilter
     */
    public function __get($name) {
        return $this->_filters->filter($name);
    }

    /**
     * 初始化活动过滤器集合。
     */
    protected function _initializeFilters() {
        $this->_filters = new ActionFilters($this);
    }

    /**
     * 根据$actionMethod参数，执行活动过滤方法。
     *
     * @param string $actionMethod 活动方法。
     *
     * @return bool
     */
    protected function _invokeFilters($actionMethod) {
        return $this->_filters->invoke($actionMethod);
    }

    /**
     * 根据$options参数，执行渲染过滤器方法。
     *
     * @param array $options 渲染选项。
     *
     * @return bool
     */
    protected function _renderFilters(array &$options) {
        return $this->_filters->render($options);
    }

    /**
     * 完整过滤器集合。
     */
    protected function _completeFilters() {
        $this->_filters->complete();
    }

    /**
     * 根据$method参数，使用控制器器功能模板填充响应信息。
     *
     * @param string $method 方法名称。
     */
    protected function _ctlTpl($method) {
        $this->render(array(
            SKIHAT_PARAM_MODULE => false,
            SKIHAT_PARAM_PACKAGE => false,
            SKIHAT_PARAM_ACTION => $method,
            SKIHAT_PARAM_CONTROLLER => '__ctltpls'
        ));
    }

    /**
     * @var bool|array 完成处理后的响应消息，默认为false。
     */
    protected $_message = false;

    /**
     * @var ActionFilters 活动过滤器集合实例。
     */
    protected $_filters = null;
}