<?php
/**
 * theme.inc 提供主题视图类声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.views
 * @version   1.0
 *
 * @created   2012-08-14
 * @modified  2013-08-18
 */

/**
 * 主题视图类，提供Skihat框架主题模板支持。
 *
 * @package skihat.views
 */
class Theme extends CollectionBase implements IView {
    /**
     * @const string 视图选项，占位符。
     */
    const OPT_PLACE = 'place';

    /**
     * @var string 视图名称，默认为false。
     */
    public $theme = false;

    /**
     * @var ApplicationRequest 应用请求。
     */
    public $request = null;

    /**
     * @var ApplicationResponse 应用响应。
     */
    public $response = null;

    /**
     * @see IView::initialize
     */
    public function initialize(Controller $controller, array $options) {
        $defaultOptions = array(
            SKIHAT_PARAM_MODULE => $controller->request->module(),
            SKIHAT_PARAM_PACKAGE => $controller->request->package(),
            SKIHAT_PARAM_CONTROLLER => $controller->request->controller(),
            SKIHAT_PARAM_ACTION => $controller->request->action(),
            self::OPT_PASS => false, self::OPT_TEMPLATE => false
        );

        $this->request = $controller->request;
        $this->response = $controller->response;

        $this->_context = $controller;
        $this->_options = array_merge($defaultOptions, $options);
    }

    /**
     * @see IView::assignAll
     */
    public function assignAll(array $vars) {
        $this->_vars = $vars;
    }

    /**
     * @see IView::message
     */
    public function message($message = null) {
        # 如果指定消息包含值。
        if ($message) {
            $this->_message = $message;
        }

        return $this->_message;
    }

    /**
     * @see IView::inflate
     */
    public function inflate() {
        $template = $this->_locateTemplates();
        $content = $this->_inflateTemplate($template);

        if ($this->_layout) {
            if ($this->_currPlace) {
                $this->_places[$this->_currPlace] = $content;
                $this->_currPlace = false;
            }

            $layout = $this->_locateLayout();
            $content = $this->_inflateTemplate($layout);
        }

        $this->response->contentBody($content);
    }

    /**
     * 根据$layout指定当前视图的布局文件。
     *
     * @param string $layout 布局文件名称。
     */
    public function layout($layout) {
        $this->_layout = $layout;
    }

    /**
     * 从文件中导入视图助手文件。
     */
    public function helpers() {
        $helpers = func_get_args();
        $paths = $this->_helperPaths();

        foreach ($helpers as $helper) {
            if (!isset($this->_helpers[$helper])) {
                $this->_helpers[$helper] = Skihat::import($helper, $paths);
            }
        }
    }

    /**
     * 根据$name和$default输出占位符的内容。
     *
     * @param string $name    占位符名称。
     * @param mixed  $default 默认值。
     */
    public function place($name, $default = '') {
        echo isset($this->_places[$name]) ? $this->_places[$name] : $default;
    }

    /**
     * 根据$text和$options填充文本内容。
     *
     * @param string      $text  文本内容。
     * @param bool|string $place 占位符名称。
     */
    public function inflateText($text, $place = false) {
        if ($place) {
            $this->_places[$place] = $text;
        } else {
            echo $text;
        }
    }

    /**
     * 根据$proc,$args和$options使用自定义方法填充视图模板。
     *
     * @param callable $proc    方法名称。
     * @param array    $args    参数集。
     * @param array    $options 选项。
     */
    public function inflateProc($proc, array  $args = array(), $options = array()) {
        # 填充基本选项信息。
        $options = $this->_inflateOptions($options);

        # 如果允许进行缓存，并且包含有缓存选项。
        if (enable_cached() && $cache = $options[self::OPT_CACHE]) {
            if (!($cc = Cache::read($cache, $options[self::OPT_CACHE_ENGINE]))) {
                $cc = $this->_inflateProc($proc, $args);
                Cache::write($cache, $cc, $options[self::OPT_CACHE_ENGINE], $options[self::OPT_CACHE_EXPIRE]);
            }
        } else {
            $cc = $this->_inflateProc($proc, $args);
        }

        if (isset($options[self::OPT_PLACE])) {
            $this->_places[$options[self::OPT_PLACE]] = $cc;
        } else {
            echo $cc;
        }
    }

    /**
     * 使用$file和$options参数填充视图模板。
     *
     * @param string $file    文件名称。
     * @param array  $options 选项。
     */
    public function inflateFile($file, $options = array()) {
        # 填充基本的选项信息。
        $options = $this->_inflateOptions($options);

        # 判断是否包含缓存设置。
        if (enable_cached() && $cache = $options[self::OPT_CACHE]) {
            if (!($cc = Cache::read($cache, $options[self::OPT_CACHE_ENGINE]))) {
                $cc = $this->_inflateFile($file);
                Cache::write($cache, $cc, $options[self::OPT_CACHE_ENGINE], $options[self::OPT_CACHE_EXPIRE]);
            }
        } else {
            $cc = $this->_inflateFile($file);
        }

        if (isset($options[self::OPT_PLACE])) {
            $this->_places[$options[self::OPT_PLACE]] = $cc;
        } else {
            echo $cc;
        }
    }

    /**
     * 根据$place启动新的占位符模板录入。
     *
     * @param string $place 占位符名称。
     */
    public function beginPlace($place) {
        if ($this->_currPlace) {
            $this->endPlace();
        }

        ob_start();
        $this->_currPlace = $place;
    }

    /**
     * 结束前一个占位符的模板录入。
     */
    public function endPlace() {
        # 如果没有指定当前占位符，直接返回值。
        if (!$this->_currPlace) return;

        $cc = ob_get_contents();
        ob_end_clean();

        $this->_places[$this->_currPlace] = $cc;
        $this->_currPlace = false;
    }

    /**
     * 根据$options生成完整的填充选项值。
     *
     * @param mixed $options 选项。
     *
     * @return array
     */
    protected function _inflateOptions($options) {
        # 如果选项为字符串。
        if (is_string($options)) {
            return array(self::OPT_PLACE => $options, self::OPT_CACHE => false);
        }

        # 如果指定有缓存名称，则进行缓存的默认值处理。
        if (isset($options[self::OPT_CACHE])) {
            # 如果指定有缓存标记，并且没有指定缓存过期时间。
            if (empty($options[self::OPT_CACHE_EXPIRE])) {
                $options[self::OPT_CACHE_EXPIRE] = Cache::DEFAULT_EXPIRE;
            }

            # 如果没有指定缓存引擎，则使用默认缓存。
            if (empty($options[self::OPT_CACHE_ENGINE])) {
                $options[self::OPT_CACHE_ENGINE] = Cache::DEFAULT_ENGINE;
            }
        } else {
            $options[self::OPT_CACHE] = false;
        }

        return $options;
    }

    /**
     * 使用$file文件填充内容，并返回。
     *
     * @param string $file 文件名称。
     *
     * @throws FileNotFoundException
     * @return string
     */
    protected function _inflateFile($file) {
        # 获取基础路径。
        $paths = $this->_basePaths();

        # 如果查找文件模板成功。
        if ($template = Skihat::locate($file, $paths, self::TEMPLATE_EXT)) {
            return $this->_inflateTemplate($template);
        }

        if (SKIHAT_DEBUG) {
            throw new FileNotFoundException('Theme:not found template ' . $file . ' in "' . implode(',', $paths) . '"');
        } else {
            trigger_error('Theme:not found template ' . $file);
        }

        return 'Theme:not found template ' . $file;
    }


    /**
     * 使用$file参数填充模板内容，并返回结果。
     *
     * @param string $file 文件地址。
     *
     * @return string
     */
    protected function _inflateTemplate($file) {
        # 生成三个局部变量，方便在模板中调用。
        $context = $this->_context;
        $request = $this->request;
        $response = $this->response;

        ob_start();
        include($file);
        $cc = ob_get_contents();
        ob_end_clean();

        return $cc;
    }


    /**
     * 根据$proc和$args执行填充方法。
     *
     * @param callable $proc 方法。
     * @param array    $args 参数。
     *
     * @return array
     */
    protected function _inflateProc($proc, array $args) {
        ob_start();
        $text = call_user_func_array($proc, $args);
        $cc = ob_get_contents();
        ob_end_clean();

        return $text . $cc;
    }

    /**
     * 获取路径信息，助手路径。
     *
     * @return array
     */
    protected function _helperPaths() {
        # 如果视图助手的路径还没有进行查找，则开始查找。
        if (!$this->_helperPaths) {
            $this->_helperPaths = $this->_defaultHelperPaths();
        }

        return $this->_helperPaths;
    }

    /**
     * 获取默认路径，视图助手默认路径。
     *
     * @return array
     */
    protected function _defaultHelperPaths() {
        $allPaths = array();

        if ($theme = $this->theme) {
            array_push($allPaths, full_path(SKIHAT_PATH_APP, 'themes', $theme, '__helpers'));
        }

        if ($module = $this->_options[SKIHAT_PARAM_MODULE]) {
            array_push($allPaths, full_path(SKIHAT_PATH_APP_MODULES, $module, 'views/__helpers'));
        }

        array_push($allPaths, full_path(SKIHAT_PATH_APP, 'views/__helpers'));
        array_push($allPaths, full_path(SKIHAT_PATH_LIBRARY, 'views/__helpers'));

        return $allPaths;
    }

    /**
     * 根据设置值，查找布局模板并返回。
     *
     * @throws FileNotFoundException 如果布局文件没有找到。
     * @return string
     */
    protected function _locateLayout() {
        $allPaths = $this->_layoutPaths();

        if ($file = Skihat::locate($this->_layout, $allPaths, self::TEMPLATE_EXT)) {
            return $file;
        }

        throw new FileNotFoundException('Theme:not found layout file "' . $this->_layout . '" in "' . implode(',', $allPaths) . '"');
    }

    /**
     * 根据当前选项信息，返回布局模板路径。
     *
     * @return array
     */
    protected function _layoutPaths() {
        # 如果布局路径还没有获取到，则获取布局模板。
        if (!$this->_layoutPaths) {
            $this->_layoutPaths = $this->_defaultLayoutPaths();
        }

        return $this->_layoutPaths;
    }

    /**
     * 根据当前选项信息，返回默认的布局路径。
     *
     * @return array
     */
    protected function _defaultLayoutPaths() {
        # 声明路径参数。
        $allPaths = array();

        if ($theme = $this->theme) {
            array_push($allPaths, full_path(SKIHAT_PATH_APP, 'themes', $theme, '__layouts'));
        }

        if ($module = $this->_options[SKIHAT_PARAM_MODULE]) {
            array_push($allPaths, full_path(SKIHAT_PATH_APP_MODULES, $module, 'views/__layouts'));
        }

        array_push($allPaths, full_path(SKIHAT_PATH_APP, 'views/__layouts'));
        array_push($allPaths, full_path(SKIHAT_PATH_LIBRARY, 'views/__layouts'));

        return $allPaths;
    }

    /**
     * 获取当前视图的模板文件。
     */
    protected function _locateTemplates() {
        $paths = $this->_templatePaths();
        $templates = $this->_templateFiles();

        foreach ($templates as $template) {
            if ($file = Skihat::locate($template, $paths, self::TEMPLATE_EXT)) {
                return $file;
            }
        }

        throw new FileNotFoundException('Theme:not found template file "' . implode(',', $templates) . '" in "' . implode(',', $paths) . '"');
    }

    /**
     * 根据主题视图选项参数，返回模板的路径集合。
     *
     * @return array
     */
    protected function _templatePaths() {
        # 如果模板路径还没有进行查询，查找模板路径。
        if (!$this->_templatePaths) {
            $this->_templatePaths = $this->_defaultTemplatePaths();
        }

        return $this->_templatePaths;
    }

    /**
     * 根据视图选项信息，返回视图默认模板路径。
     *
     * @return array
     */
    protected function _defaultTemplatePaths() {
        $allPaths = array();

        if ($module = $this->_options[SKIHAT_PARAM_MODULE]) {
            if ($theme = $this->theme) {
                array_push($allPaths, full_path(SKIHAT_PATH_APP, 'themes', $theme, $module));
            }

            array_push($allPaths, full_path(SKIHAT_PATH_APP, 'views', $module));
            array_push($allPaths, full_path(SKIHAT_PATH_APP_MODULES, $module, 'views'));
        } else {
            if ($theme = $this->theme) {
                array_push($allPaths, full_path(SKIHAT_PATH_APP, 'themes', $theme));
            }

            array_push($allPaths, full_path(SKIHAT_PATH_APP, 'views'));
            array_push($allPaths, full_path(SKIHAT_PATH_LIBRARY, 'views'));
        }

        return $allPaths;
    }

    /**
     * 根据主题视图选项参数，返回模板文件路径。
     *
     * @return string|array
     */
    protected function _templateFiles() {
        # 如果在选项中已经指定了模板信息。
        if ($templates = $this->_options[self::OPT_TEMPLATE]) {
            return (array)$templates;
        }

        return $this->_defaultTemplateFiles();
    }

    /**
     * 根据视图选项信息，返回默认模板文件集。
     *
     * @return array
     */
    protected function _defaultTemplateFiles() {
        # 生成模板初始化信息。
        $template = $this->_options[SKIHAT_PARAM_CONTROLLER];
        if ($package = $this->_options[SKIHAT_PARAM_PACKAGE]) {
            $template = $package . SKIHAT_PACKAGE_SEPARATOR . $template;
        }

        # 获取活动信息。
        $action = $this->_options[SKIHAT_PARAM_ACTION];

        # 判断是否包含pass参数选项。
        if ($pass = $this->_options['pass']) {
            if (!is_array($pass)) {
                return array(
                    $template . SKIHAT_PACKAGE_SEPARATOR . $pass . SKIHAT_PACKAGE_SEPARATOR . $action,
                    $template . SKIHAT_PACKAGE_SEPARATOR . $action);
            }

            $templates = array();
            while ($pass) {
                array_push($templates, $template . SKIHAT_PACKAGE_SEPARATOR . implode(SKIHAT_PACKAGE_SEPARATOR, $pass) . SKIHAT_PACKAGE_SEPARATOR . $action);
                array_pop($pass);
            }

            array_push($templates, $template . SKIHAT_PACKAGE_SEPARATOR . $action);
            return $templates;
        }

        return array($template . SKIHAT_PACKAGE_SEPARATOR . $action);
    }

    /**
     * 返回模板的基础地址路径集合。
     *
     * @return array
     */
    protected function _basePaths() {
        # 如果还没有生成基础地址，则生成地址。
        if (!$this->_basePaths) {
            $this->_basePaths = $this->_defaultBasePaths();
        }

        return $this->_basePaths;
    }

    /**
     * 根据视图选项，返回基础路径。
     *
     * @return array
     */
    protected function _defaultBasePaths() {
        # 如果在选项中饮食基础路径。
        if (isset($this->_options[self::OPT_BASE_PATHS])) {
            return (array)$this->_options[self::OPT_BASE_PATHS];
        }

        $allPaths = array();

        # 如果当前视图有主题信息。
        if ($theme = $this->theme) {
            array_push($allPaths, full_path(SKIHAT_PATH_APP, 'themes', $theme));
        }
        array_push($allPaths, full_path(SKIHAT_PATH_APP, 'views'));

        # 如果包含模块。
        if ($module = $this->_options[SKIHAT_PARAM_MODULE]) {
            array_push($allPaths, full_path(SKIHAT_PATH_APP_MODULES, $module, 'views'));
        }
        array_push($allPaths, full_path(SKIHAT_PATH_LIBRARY, 'views'));

        return $allPaths;
    }

    /**
     * @var Controller 视图相关控制器。
     */
    protected $_context = null;

    /**
     * @var array 视图选项。
     */
    protected $_options = array();

    /**
     * @var array 视图响应消息成员。
     */
    protected $_message = null;

    /**
     * @var bool|string 视图布局文件。
     */
    protected $_layout = false;

    /**
     * @var array 视图占位符。
     */
    protected $_places = array();

    /**
     * @var bool|string 当前占位符名称。
     */
    protected $_currPlace = false;

    /**
     * @var array 视图助手信息。
     */
    protected $_helpers = array();

    /**
     * @var string|bool 视图助手路径缓存字段。
     */
    protected $_helperPaths = false;

    /**
     * @var mixed 模板路径值，保存模板的路径信息。
     */
    protected $_templatePaths = false;

    /**
     * @var mixed 布局模板路径。
     */
    protected $_layoutPaths = false;

    /**
     * @var mixed 模板的基础地址。
     */
    protected $_basePaths = false;
}