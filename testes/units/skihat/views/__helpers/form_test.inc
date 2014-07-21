<?php
/**
 * form_test.inc 提供表单生成器的单元测试服务。
 *
 * @copyright   2012-2013 Skihat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      Skihat Team
 *
 * @package     skihat.views.__helpers
 * @version     1.0
 *
 * @create      2012-10-16
 * @modify      2013-09-18
 */

# 导入测试启动文件。
require_once(dirname(dirname(dirname(dirname(__FILE__)))) . '/startup.inc');

# 导入关联类和被测试类
Skihat::import('views.view', SKIHAT_PATH_LIBRARY);
Skihat::import('views.theme', SKIHAT_PATH_LIBRARY);
Skihat::import('views.__helpers.form', SKIHAT_PATH_LIBRARY);

Skihat::import('models.model', SKIHAT_PATH_LIBRARY);
Skihat::import('controllers.controller', SKIHAT_PATH_LIBRARY);

Skihat::import('applications.application_response', SKIHAT_PATH_LIBRARY);
Skihat::import('applications.application_request', SKIHAT_PATH_LIBRARY);


# 导入和初始化路由器。
Skihat::import('routers.router', SKIHAT_PATH_LIBRARY);

# 初始化路由配置
Router::domain(SKIHAT_CURRENT_DOMAIN)->rule('#NormalRouterRule');
Router::defaultParams(array(), array());

/**
 * 表单测试服务，提供表单生成器的测试服务。
 *
 * @package skihat.views.__helpers
 */
class FormTest extends PHPUnit_Framework_TestCase {
    /**
     * 测试表单开始标志。
     */
    #public function testBegin() {
    #    $fb = new FormBuilder($this->_view());

    #    $this->assertEquals('<form action="/" method="POST" accept-charset="UTF-8">' . '<div style="display:none;"><input type="hidden" name="__method__" value="POST" /></div>', $fb->begin());
    # }

    /**
     * 测试表单开始标志，包含选项。
     */
    # public function testBeginForAttrs() {
    #    $fb = new FormBuilder($this->_view());
    #     $html = $fb->begin(array('action' => 'ab.php',
    #                              'method' => 'PUT'));

    #    $this->assertEquals('<form action="ab.php" method="POST" accept-charset="UTF-8">' . '<div style="display:none;"><input type="hidden" name="__method__" value="PUT" /></div>', $html);
    # }

    /**
     * 测试表单开始标志，使用GET方法。
     */
    #public function testBeginForGetMethod() {
    #    $fb = new FormBuilder($this->_view());

    #    $this->assertEquals('<form method="GET" action="/" accept-charset="UTF-8">', $fb->begin(array('method' => 'GET')));
    #}

    /**
     * 测试生成结束标志。
     */
    public function testEnd() {
        $fb = new FormBuilder($this->_view());
        $this->assertEquals('</form>', $fb->end());
    }

    /**
     * 测试生成label。
     */
    public function testLabel() {
        $fb = new FormBuilder($this->_view());

        $this->assertEquals('<label id="user-name-label" for="user-name">user name</label>', $fb->label('user.name'));
    }

    /**
     * 测试生成label，指定字符串选项。
     */
    public function testLabelForStringAttrs() {
        $fb = new FormBuilder($this->_view());
        $html = $fb->label('user.name', 'User Name:');

        $this->assertEquals('<label id="user-name-label" for="user-name">User Name:</label>', $html);
    }

    /**
     * 测试生成label，指定选项标题。
     */
    public function testLabelForAttrsTitle() {
        $fb = new FormBuilder($this->_view());
        $html = $fb->label('user.name', array('title' => 'User Name:'));

        $this->assertEquals('<label id="user-name-label" for="user-name">User Name:</label>', $html);
    }

    /**
     * 测试生成label，指定选项。
     */
    public function testLabelForAttrs() {
        $fb = new FormBuilder($this->_view());
        $html = $fb->label('user.name', array('id' => 'user-name-title',
            'class' => 'control-label',
            'for' => 'user-name-text'));

        $this->assertEquals('<label id="user-name-title" class="control-label" for="user-name-text">user name</label>', $html);
    }

    /**
     * 测试生成隐藏域录入框。
     */
    public function testHidden() {
        $fb = new FormBuilder($this->_view());

        $this->assertEquals('<input type="hidden" id="user-name" name="user[name]" />', $fb->hidden('user.name'));
    }

    /**
     * 测试生成隐藏域，指定非数组选项。
     */
    public function testHiddenForNotArrayAttrs() {
        $fb = new FormBuilder($this->_view());

        $html = $fb->hidden('user.name', 'hello world');
        $this->assertEquals('<input type="hidden" value="hello world" id="user-name" name="user[name]" />', $html);
    }

    /**
     * 测试生成隐藏域，指定选项。
     */
    public function testHiddenForAttrs() {
        $fb = new FormBuilder($this->_view());

        $html = $fb->hidden('user.name', array('id' => 'user_name',
            'name' => 'user_name',
            'class' => 'hide',
            'value' => 'hello world'));
        $this->assertEquals('<input type="hidden" id="user_name" name="user_name" class="hide" value="hello world" />', $html);
    }

    /**
     * 测试生成隐藏域，指定视图变量。
     */
    public function testHiddenForViewVars() {
        $fb = new FormBuilder($this->_view());

        $html = $fb->hidden('group.name');
        $this->assertEquals('<input type="hidden" id="group-name" name="group[name]" value="system" />', $html);
    }

    /**
     * 测试生成隐藏域，指定格式器。
     */
    public function testHiddenForFormatter() {
        $fb = new FormBuilder($this->_view());

        $html = $fb->hidden('group.created', array('formatter' => 'formatter'));
        $this->assertEquals('<input type="hidden" id="group-created" name="group[created]" value="1970-01-01" />', $html);

        $html = $fb->hidden('group.created', array('formatter' => 'formatter', 'value' => 10000));
        $this->assertEquals('<input type="hidden" value="1970-01-01" id="group-created" name="group[created]" />', $html);
    }

    /**
     * 测试生成隐藏域，使用模型变量。
     */
    public function testHiddenForModelVars() {
        $this->_init();
        $fb = new FormBuilder($this->_modelView());

        $html = $fb->hidden('user.name');
        $this->assertEquals('<input type="hidden" id="user-name" name="user[name]" value="skihat2" />', $html);
    }

    /**
     * 测试生成checkbox录入框。
     */
    public function testCheckbox() {
        $fb = new FormBuilder($this->_view());
        $html = $fb->checkbox('user.allow', '1');

        $this->assertEquals('<input type="hidden" value="0" id="user-allow" name="user[allow]" /><input type="checkbox" value="1" id="user-allow" name="user[allow]" />', $html);
    }

    /**
     * 测试生成checkbox录入框，指定模型视图。
     */
    public function testCheckboxForModelView() {
        $fb = new FormBuilder($this->_modelView());
        $html = $fb->checkbox('user.allow', '1');

        $this->assertEquals('<input type="hidden" value="0" id="user-allow" name="user[allow]" /><input type="checkbox" value="1" id="user-allow" name="user[allow]" checked="1" />', $html);
    }

    /**
     * 测试生成单选框。
     */
    public function testRadio() {
        $fb = new FormBuilder($this->_view());
        $html = $fb->radio('user.allow', 1);

        $this->assertEquals('<input type="radio" value="1" id="user-allow-1" name="user[allow]" />', $html);
    }

    /**
     * 测试生成单选框。
     */
    public function testRadioViewModel() {
        $fb = new FormBuilder($this->_modelView());
        $html = $fb->radio('user.allow', 1);

        $this->assertEquals('<input type="radio" value="1" id="user-allow-1" name="user[allow]" checked="1" />', $html);
    }

    /**
     * 测试生成单先框，非选中模式。
     */
    public function testRadioViewModelForNotChecked() {
        $fb = new FormBuilder($this->_view());
        $html = $fb->radio('user.allow', 3);

        $this->assertEquals('<input type="radio" value="3" id="user-allow-3" name="user[allow]" />', $html);
    }

    /**
     * 测试文本域。
     */
    public function testTextarea() {
        $fb = new FormBuilder($this->_view());
        $html = $fb->textarea('user.description');

        $this->assertEquals('<textarea id="user-description" name="user[description]"></textarea>', $html);
    }

    /**
     * 测试文本字符选项，字符串选项。
     */
    public function testTextareaForStringAttrs() {
        $fb = new FormBuilder($this->_view());
        $html = $fb->textarea('user.description', 'Hello World');

        $this->assertEquals('<textarea id="user-description" name="user[description]">Hello World</textarea>', $html);
    }

    /**
     * 测试文本域，指定其它选项。
     */
    public function testTextareaForAttrs() {
        $fb = new FormBuilder($this->_view());
        $html =
            $fb->textarea('user.description', array('value' => 'Hello World', 'id' => 'user_des', 'class' => 'la'));

        $this->assertEquals('<textarea id="user_des" class="la" name="user[description]">Hello World</textarea>', $html);
    }

    /**
     * @return Theme
     */
    protected function _view() {
        $view = new Theme();
        $vars = array(
            'group' => array(
                'name' => 'system',
                'allow' => 1,
                'created' => 0
            )
        );
        $view->initialize(new FormTestContext(), array());
        $view->assignAll($vars);

        return $view;
    }

    protected function _modelView() {
        $view = new Theme();
        $vars = array('user' => FormTestUser::fetch(array('name' => 'skihat2')));
        $view->initialize(new FormTestContext(), array());
        $view->assignAll($vars);

        return $view;
    }

    protected function _init() {
        $link = new PDO(MYSQL_DSN, MYSQL_USER, MYSQL_PASS);
        $link->exec('DELETE FROM `user`');

        $stmt = $link->prepare('INSERT INTO `user`(`name`,`description`,`allow`,`group`) VALUES(?,?,1,?);');
        $stmt->execute(array('skihat', 'skihat example', 1));
        $stmt->execute(array('skihat2', 'skihat example 2', 1));
        $stmt->execute(array('skihat3', 'skihat example 3', 1));
    }


    public function setUp() {
        # 设置数据库信息。
        Skihat::write('kernels/databases', array(
                'default' => array(
                    SKIHAT_IOC_CLASS => 'kernels.databases.engines.mysql_pdo_engine#MySqlPdoEngine',
                    'conf' => MYSQL_CONF
                )
            )
        );

        Skihat::register(array(SKIHAT_PATH_LIBRARY));
        Database::initialize();
    }
}

function formatter($created) {
    return date('Y-m-d', $created);
}

class FormTestContext extends Controller {
    public $request = null;
    public $response = null;

    public function __construct() {
        $this->request = new ApplicationRequest();
        $this->response = new ApplicationResponse();
    }
}

class FormTestUser extends Model {
    public static function config() {
        return array(self::META_TABLE => 'user',
            self::META_TABLE_ALIAS => 'User');
    }
}
