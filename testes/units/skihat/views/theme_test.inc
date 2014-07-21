<?php
/**
 * theme_test.inc 提供主题视图类单元测试声明。
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

# 引用测试启动文件。
require_once(dirname(dirname(dirname(__FILE__))) . '/startup.inc');

# 引用被测试类。
Skihat::import('applications.application_request', SKIHAT_PATH_LIBRARY);
Skihat::import('applications.application_response', SKIHAT_PATH_LIBRARY);
Skihat::import('controllers.controller', SKIHAT_PATH_LIBRARY);

Skihat::import('views.view', SKIHAT_PATH_LIBRARY);
Skihat::import('views.theme', SKIHAT_PATH_LIBRARY);

/**
 * 主题视图测试类，提供Theme类的单元测试服务。
 *
 * @package skihat.views
 */
class ThemeViewTest extends PHPUnit_Framework_TestCase {
    /**
     * 测试使用选项参数。
     */
    public function testInitialize() {
        $theme = $this->_createTheme(
            array(SKIHAT_PARAM_MODULE => 'core'),
            array(SKIHAT_PARAM_MODULE => 'nuke')
        );

        $options = $theme->options();
        $this->assertEquals(6, count($options));

        $this->assertEquals('nuke', $options[SKIHAT_PARAM_MODULE]);
        $this->assertEquals('index', $options[SKIHAT_PARAM_CONTROLLER]);
        $this->assertEquals('index', $options[SKIHAT_PARAM_ACTION]);

        $this->assertFalse($options[SKIHAT_PARAM_PACKAGE]);
        $this->assertFalse($options[IView::OPT_PASS]);
        $this->assertFalse($options[IView::OPT_TEMPLATE]);
    }

    /**
     * 测试使用assignAll分配视图变量。
     */
    public function testAssignAll() {
        $theme = $this->_createTheme();
        $theme->assignAll(array('name' => 'skihat', 'version' => 1));

        $this->assertEquals('skihat', $theme['name']);
        $this->assertEquals(1, $theme['version']);
    }

    /**
     * 测试导入助手。
     */
    public function testHelpers() {
        $theme = $this->_createTheme();

        $this->assertFalse(function_exists('default_helpers'));
        $theme->helpers('default');

        $this->assertTrue(function_exists('default_helpers'));
    }

    /**
     * 测试助手路径。
     */
    public function testHelperPaths() {
        $theme = $this->_createTheme();

        $paths = $theme->helperPaths();
        $this->assertEquals(2, count($paths));

        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views/__helpers'), realpath($paths[0]));
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views/__helpers'), realpath($paths[1]));
    }

    /**
     * 测试导入助手，使用模板。
     */
    public function testHelpersForTheme() {
        $theme = $this->_createTheme();
        $theme->theme = 'default';

        $this->assertFalse(function_exists('default_theme_helpers'));
        $theme->helpers('default_theme');
        $this->assertTrue(function_exists('default_theme_helpers'));
    }

    /**
     * 测试助手路径，使用主题。
     */
    public function testHelperPathsForTheme() {
        $theme = $this->_createTheme();
        $theme->theme = 'default';

        $paths = $theme->helperPaths();

        $this->assertEquals(3, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/themes/default/__helpers'), realpath($paths[0]));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views/__helpers'), realpath($paths[1]));
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views/__helpers'), realpath($paths[2]));
    }

    /**
     * 使用导入助手，使用模块。
     */
    public function testHelpersForModule() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_MODULE => 'core'));

        $this->assertFalse(function_exists('core_module_helpers'));
        $theme->helpers('core_module');
        $this->assertTrue(function_exists('core_module_helpers'));
    }

    /**
     * 测试助手路径，包含模块。
     */
    public function testHelperPathsForModule() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_MODULE => 'core'));

        $paths = $theme->helperPaths();
        $this->assertEquals(realpath(SKIHAT_PATH_APP_MODULES . '/core/views/__helpers'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views/__helpers'), $paths[1]);
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views/__helpers'), $paths[2]);
    }

    /**
     * 测试助手路径，包含主题和模块。
     */
    public function testHelperPathsForThemeAndModule() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_MODULE => 'core'));
        $theme->theme = 'default';

        $paths = $theme->helperPaths();

        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/themes/default/__helpers'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP_MODULES . '/core/views/__helpers'), $paths[1]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views/__helpers'), $paths[2]);
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views/__helpers'), $paths[3]);
    }

    /**
     * 测试导入助手，数组参数。
     */
    public function testHelpersForArray() {
        $theme = $this->_createTheme();
        $theme->helpers('core', 'form');

        $this->assertTrue(function_exists('link_to'));
        $this->assertTrue(class_exists('FormBuilder'));
    }

    /**
     * 测试导入助手，导入失败。
     *
     * @expectedException FileNotFoundException
     */
    public function testHelpersForFail() {
        $theme = $this->_createTheme();

        $theme->helpers('not-exists');
    }

    /**
     * 测试填充模板，指定活动。
     */
    public function testInflateForAction() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'edit'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('units.app.views.index.edit.stp', $resp->contentBody());
    }

    /**
     * 测试填充模板，指定控制器。
     */
    public function testInflateForController() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_CONTROLLER => 'users'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('units.app.views.users.index.stp', $resp->contentBody());
    }

    /**
     * 测试获取模板路径。
     */
    public function testTemplatePaths() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_CONTROLLER => 'users', SKIHAT_PARAM_ACTION => 'edit'));
        $paths = $theme->templatePaths();
        $files = $theme->templateFiles();

        $this->assertEquals(2, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views'), $paths[1]);

        $this->assertEquals(1, count($files));
        $this->assertEquals('users.edit', $files[0]);
    }

    /**
     * 测试填充模板，指定包。
     */
    public function testInflateForPackage() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_PACKAGE => 'admins'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('units.app.views.admins.index.index.stp', $resp->contentBody());
    }

    /**
     * 测试模板路径，指定包。
     */
    public function testTemplatePathsForPackage() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_PACKAGE => 'admins'));
        $paths = $theme->templatePaths();
        $files = $theme->templateFiles();

        $this->assertEquals(2, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views'), $paths[1]);

        $this->assertEquals(1, count($files));
        $this->assertEquals('admins.index.index', $files[0]);
    }

    /**
     * 测试填充模板，指定模块。
     */
    public function testInflateForModule() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_MODULE => 'core'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('units.app.modules.core.views.index.index.stp', $resp->contentBody());
    }

    /**
     * 测试模板路径，指定模块。
     */
    public function testTemplatePathsForModule() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_MODULE => 'core'));
        $paths = $theme->templatePaths();
        $files = $theme->templateFiles();

        $this->assertEquals(2, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views/core'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP_MODULES . '/core/views'), $paths[1]);

        $this->assertEquals(1, count($files));
        $this->assertEquals('index.index', $files[0]);
    }

    /**
     * 测试填充模板，指定主题。
     */
    public function testInflateForTheme() {
        $theme = $this->_createTheme();
        $resp = $theme->response();

        $theme->theme = 'default';
        $theme->inflate();

        $this->assertEquals('units.app.themes.default.index.index', $resp->contentBody());
    }

    /**
     * 测试模板路径，指定主题。
     */
    public function testTemplatePathsForTheme() {
        $theme = $this->_createTheme();
        $theme->theme = 'default';

        $paths = $theme->templatePaths();
        $files = $theme->templateFiles();

        $this->assertEquals(3, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/themes/default'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views'), $paths[1]);
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views'), $paths[2]);

        $this->assertEquals(1, count($files));
        $this->assertEquals('index.index', $files[0]);
    }

    /**
     * 测试填充主题，指定模块。
     */
    public function testInflateForThemeAndModule() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_MODULE => 'core'));
        $resp = $theme->response();

        $theme->theme = 'default';
        $theme->inflate();

        $this->assertEquals('units.app.themes.default.core.index.index', $resp->contentBody());
    }

    /**
     * 测试模板路径，指定主题和模块。
     */
    public function testTemplatePathsForThemeAndModule() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_MODULE => 'core'));
        $theme->theme = 'default';

        $paths = $theme->templatePaths();
        $files = $theme->templateFiles();

        $this->assertEquals(3, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/themes/default/core'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views/core'), $paths[1]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP_MODULES . '/core/views'), $paths[2]);

        $this->assertEquals(1, count($files));
        $this->assertEquals('index.index', $files[0]);
    }

    /**
     * 测试填充模板，指定主题，模块和包。
     */
    public function testInflateForThemeAndModuleAndPackage() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_MODULE => 'core', SKIHAT_PARAM_PACKAGE => 'admins'));
        $resp = $theme->response();
        $theme->theme = 'default';

        $theme->inflate();

        $this->assertEquals('units.app.themes.default.core.admins.index.index.stp', $resp->contentBody());
    }

    /**
     * 测试模板路径，指定主题，模块和包。
     */
    public function testTemplatePathsForThemeAndModuleAndPackage() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_MODULE => 'core', SKIHAT_PARAM_PACKAGE => 'admins'));
        $theme->theme = 'default';

        $paths = $theme->templatePaths();
        $files = $theme->templateFiles();

        $this->assertEquals(3, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/themes/default/core'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views/core'), $paths[1]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP_MODULES . '/core/views'), $paths[2]);

        $this->assertEquals(1, count($files));
        $this->assertEquals('admins.index.index', $files[0]);
    }

    /**
     * 测试填充，指定主题和包。
     */
    public function testInflateForThemeAndPackage() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_PACKAGE => 'admins'));
        $theme->theme = 'default';

        $theme->inflate();
        $this->assertEquals('units.app.themes.default.admins.index.index.stp', $theme->response()->contentBody());
    }

    /**
     * 测试模板路径，指定主题和包。
     */
    public function testTemplatePathsForThemeAndPackage() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_PACKAGE => 'admins'));
        $theme->theme = 'default';

        $paths = $theme->templatePaths();
        $files = $theme->templateFiles();

        $this->assertEquals(3, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/themes/default'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views'), $paths[1]);
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views'), $paths[2]);

        $this->assertEquals(1, count($files));
        $this->assertEquals('admins.index.index', $files[0]);
    }

    /**
     * 测试填充模板，使用pass参数。
     */
    public function testInflateForPass() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'edit'), array('pass' => 'ajax'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('units.app.views.index.ajax.edit.stp', $resp->contentBody());
    }

    /**
     * 测试填充模板，使用pass参数。
     */
    public function testTemplatePathsForPass() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'edit'), array('pass' => 'ajax'));
        $files = $theme->templateFiles();

        $this->assertEquals(2, count($files));
        $this->assertEquals('index.ajax.edit', $files[0]);
        $this->assertEquals('index.edit', $files[1]);
    }

    /**
     * 测试填充模板，使用pass数组参数。
     */
    public function testInflateForPassArray() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'edit'), array('pass' => array('ajax')));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('units.app.views.index.ajax.edit.stp', $resp->contentBody());
    }

    /**
     * 测试模板路径，指定pass数组参数。
     */
    public function testTemplatePathsForPassArray() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'edit'), array('pass' => array('ajax')));
        $files = $theme->templateFiles();

        $this->assertEquals(2, count($files));
        $this->assertEquals('index.ajax.edit', $files[0]);
        $this->assertEquals('index.edit', $files[1]);
    }

    /**
     * 测试填充模板，使用template模板参数。
     */
    public function testInflateForTemplate() {
        $theme = $this->_createTheme(array(), array('template' => 'index.ajax.edit'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('units.app.views.index.ajax.edit.stp', $resp->contentBody());
    }

    /**
     * 测试填充模板，使用template数组参数。
     */
    public function testInflateForTemplateArray() {
        $theme = $this->_createTheme(array(), array('template' => array('index.edit', 'index.ajax.edit')));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('units.app.views.index.edit.stp', $resp->contentBody());
    }

    /**
     * 测试模板路径，指定模板参数。
     */
    public function testTemplatePathsForTemplate() {
        $theme = $this->_createTheme(array(), array('template' => array('index.edit', 'index.ajax.edit')));
        $files = $theme->templateFiles();

        $this->assertEquals(2, count($files));
        $this->assertEquals('index.edit', $files[0]);
        $this->assertEquals('index.ajax.edit', $files[1]);
    }

    /**
     * 测试填充模板，使用template并包含主题。
     */
    public function testInflateForTemplateAndTheme() {
        $theme = $this->_createTheme(array(), array('template' => 'admins.index.index'));
        $resp = $theme->response();

        $theme->theme = 'default';
        $theme->inflate();

        $this->assertEquals('units.app.themes.default.admins.index.index.stp', $resp->contentBody());
    }

    /**
     * 测试使用模板。
     */
    public function testLayout() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'layout'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('units.app.views.__layouts.default.stp', $resp->contentBody());
    }

    /**
     * 测试布局路径。
     */
    public function testLayoutPaths() {
        $theme = $this->_createTheme();
        $paths = $theme->layoutPaths();

        $this->assertEquals(2, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views/__layouts'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views/__layouts'), $paths[1]);
    }

    /**
     * 测试使用模板，包含主题。
     */
    public function testLayoutForTheme() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'layout'));
        $resp = $theme->response();
        $theme->theme = 'default';
        $theme->inflate();

        $this->assertEquals('units.app.themes.default.__layouts.default.stp', $resp->contentBody());
    }

    /**
     * 测试布局路径，指定主题。
     */
    public function testLayoutPathsForTheme() {
        $theme = $this->_createTheme();
        $theme->theme = 'default';
        $paths = $theme->layoutPaths();

        $this->assertEquals(3, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/themes/default/__layouts'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views/__layouts'), $paths[1]);
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views/__layouts'), $paths[2]);
    }

    /**
     * 测试布局路径，指定模块。
     */
    public function testLayoutPathsForModule() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_MODULE => 'core'));
        $paths = $theme->layoutPaths();

        $this->assertEquals(3, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP_MODULES . '/core/views/__layouts'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views/__layouts'), $paths[1]);
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views/__layouts'), $paths[2]);
    }

    /**
     * 测试布局路径，指定主题和模块。
     */
    public function testLayoutPathsForThemeAndModule() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_MODULE => 'core'));
        $theme->theme = 'default';
        $paths = $theme->layoutPaths();

        $this->assertEquals(4, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/themes/default/__layouts'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP_MODULES . '/core/views/__layouts'), $paths[1]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views/__layouts'), $paths[2]);
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views/__layouts'), $paths[3]);
    }

    /**
     * 测试填充文本。
     */
    public function testInflateText() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'inflate_text'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('inflate_text', $resp->contentBody());
    }

    /**
     * 测试填充文本，使用占位符标记。
     */
    public function testInflateTextForPlace() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'inflate_text_place'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('begin:inflate_text_place:end', $resp->contentBody());
    }

    /**
     * 测试填充模板，使用自定义方法。
     */
    public function testInflateProc() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'inflate_proc'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('inflate_proc', $resp->contentBody());
    }

    /**
     * 测试填充模板，使用自定义方法填充到占位符中。
     */
    public function testInflateProcForPlace() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'inflate_proc_place'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('begin:inflate_proc_place:end', $resp->contentBody());
    }

    /**
     * 测试填充模板，使用另外的模板文件。
     */
    public function testInflateFile() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'inflate_file'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('inflate_file', $resp->contentBody());
    }

    /**
     * 测试填充模板，
     */
    public function testInflateFileForPlace() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'inflate_file_place'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('begin:inflate_file:end', $resp->contentBody());
    }

    /**
     * 测试主题填充。
     */
    public function testInflateFileForTheme() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'inflate_file_theme'));
        $resp = $theme->response();
        $theme->theme = 'default';
        $theme->inflate();

        $this->assertEquals('inflate_file_theme', $resp->contentBody());
    }

    /**
     * 测试文件填充，使用模块。
     */
    public function testInflateFileForModule() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'inflate_file_module', SKIHAT_PARAM_MODULE => 'core'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('inflate_file_module', $resp->contentBody());
    }

    /**
     * 测试文件填充，使用模块和主题。
     */
    public function testInflateFileForThemeAndModule() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'inflate_file_theme_and_module', SKIHAT_PARAM_MODULE => 'core'));
        $resp = $theme->response();

        $theme->theme = 'default';
        $theme->inflate();

        $this->assertEquals('inflate_file_theme_and_module', $resp->contentBody());
    }

    /**
     * 测试基础路径。
     */
    public function testBasePaths() {
        $theme = $this->_createTheme();
        $paths = $theme->basePaths();

        $this->assertEquals(2, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views'), $paths[1]);
    }

    /**
     * 测试基础路径，包含主题信息。
     */
    public function testBasePathsForTheme() {
        $theme = $this->_createTheme();
        $theme->theme = 'default';

        $paths = $theme->basePaths();

        $this->assertEquals(3, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/themes/default'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views'), $paths[1]);
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views'), $paths[2]);
    }

    /**
     * 测试基础路径，包含主题信息。
     */
    public function testBasePathsForThemeAndModule() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_MODULE => 'core'));
        $theme->theme = 'default';

        $paths = $theme->basePaths();

        $this->assertEquals(4, count($paths));
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/themes/default'), $paths[0]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP . '/views'), $paths[1]);
        $this->assertEquals(realpath(SKIHAT_PATH_APP_MODULES . '/core/views'),$paths[2]);
        $this->assertEquals(realpath(SKIHAT_PATH_LIBRARY . '/views'), $paths[3]);
    }

    /**
     * 测试填充模板，使用place方法。
     */
    public function testBeginPlace() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'begin_place'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('begin:begin_place:end', $resp->contentBody());
    }

    /**
     * 测试填充模板，却失EndPlace方法。
     */
    public function testInflateForMissingEndPlace() {
        $theme = $this->_createTheme(array(SKIHAT_PARAM_ACTION => 'missing_end_place'));
        $resp = $theme->response();
        $theme->inflate();

        $this->assertEquals('begin:missing_end_place:end', str_replace(array("\r", "\n"), '', $resp->contentBody()));
    }

    /**
     * 根据$params创建控制器实例。
     *
     * @param array $params  请求参数。
     * @param array $options 选项参数。
     *
     * @return DefaultTheme
     */
    protected function _createTheme(array $params = array(), array $options = array()) {
        $ctlObj = new  DefaultThemeController(new ApplicationRequest(), new ApplicationResponse());
        $theme = new DefaultTheme();

        $ctlObj->request->initialize($params);
        $theme->initialize($ctlObj, $options);

        return $theme;
    }
}

/**
 * 默认主题视图类，提供Theme单元测试所需的服务。
 *
 * @package skihat.views
 */
class DefaultTheme extends Theme {
    public function options() {
        return $this->_options;
    }

    public function controller() {
        return $this->_context;
    }

    public function response() {
        return $this->_context->response;
    }

    public function helperPaths() {
        return $this->_helperPaths();
    }

    public function templatePaths() {
        return $this->_templatePaths();
    }

    public function templateFiles() {
        return $this->_templateFiles();
    }

    public function layoutPaths() {
        return $this->_layoutPaths();
    }

    public function basePaths() {
        return $this->_basePaths();
    }
}

/**
 * 默认主题控制器，提供Theme单元测试所需的控制器服务支持。
 *
 * @package skihat.views
 */
class DefaultThemeController extends Controller {
    public function __construct(ApplicationRequest $request, ApplicationResponse $response) {
        $this->_initialize($request, $response);
    }
}