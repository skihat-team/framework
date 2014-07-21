<?php
/**
 * skihat_test.inc 提供SkihatTest测试的声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels
 * @version   1.0
 *
 * @created   2012-06-12
 * @modified  2013-08-02
 */

require_once(dirname(dirname(dirname(__FILE__))) . '/startup.inc');

define('SKIHAT_PATH_LOCALS', SKIHAT_PATH_TESTES_UNITS . '/locals');

define('SKIHAT_PATH_LOCALS_APP', SKIHAT_PATH_LOCALS . '/app');
define('SKIHAT_PATH_LOCALS_LIBRARY', SKIHAT_PATH_LOCALS . '/skihat');
define('SKIHAT_PATH_LOCALS_VENDOR', SKIHAT_PATH_LOCALS . '/vendor');


/**
 * SkihatTest类，提供Skihat类单元测试服务。
 *
 * @package skihat.kernels
 */
class SkihatTest extends PHPUnit_Framework_TestCase {

    public function setUp() {
        SkihatMock::reset();
    }

    /**
     * 测试注册全局路径。
     */
    public function testRegister() {
        SkihatMock::register(array(SKIHAT_PATH_APP, SKIHAT_PATH_LIBRARY));

        $paths = SkihatMock::paths();
        $this->assertEquals(SKIHAT_PATH_APP, $paths[0]);
        $this->assertEquals(SKIHAT_PATH_LIBRARY, $paths[1]);
    }

    /**
     * 测试定入配置信息。
     */
    public function testWriteAndRead() {
        SkihatMock::write('name', 'hello skihat');
        SkihatMock::write('work', 'hello framework');

        $this->assertEquals('hello skihat', SkihatMock::read('name'));
        $this->assertEquals('hello framework', SkihatMock::read('work'));
    }

    /**
     * 测试写入配置信息，覆盖原有值。
     */
    public function testWriteForOverride() {
        SkihatMock::write('work', 'hello framework');

        SkihatMock::write('name', 'hello world');
        SkihatMock::write('name', 'hello skihat');

        $this->assertEquals('hello skihat', SkihatMock::read('name'));
        $this->assertEquals('hello framework', SkihatMock::read('work'));
    }

    /**
     * 测试读取配置信息，读取默认值。
     */
    public function testReadForDefault() {
        $this->assertFalse(SkihatMock::read('name'));
        $this->assertEquals('hello ski', SkihatMock::read('name', 'hello ski'));
    }

    /**
     * 测试写入调试信息。
     */
    public function testDebug() {
        SkihatMock::debug('SQL', 'SELECT * FROM users');
        SkihatMock::debug('Cache', 'Write cache');

        $debugs = SkihatMock::debugs();
        $this->assertEquals('<li><b>SQL</b>:SELECT * FROM users</li>', $debugs['SQL'][0]);
        $this->assertEquals('<li><b>Cache</b>:Write cache</li>', $debugs['Cache'][0]);
    }

    /**
     * 测试重复写入调试，重复写入信息。
     */
    public function testDebugForOverride() {
        SkihatMock::debug('SQL', 'SELECT * FROM users');
        SkihatMock::debug('SQL', 'SELECT * FROM system');

        $debugs = SkihatMock::debugs();
        $this->assertEquals('<li><b>SQL</b>:SELECT * FROM users</li>', $debugs['SQL'][0]);
        $this->assertEquals('<li><b>SQL</b>:SELECT * FROM system</li>', $debugs['SQL'][1]);
    }

    /**
     * 测试调试信息输出。
     */
    public function testDebugLines() {
        SkihatMock::debug('SQL', 'SELECT * FROM users');

        $lines = SkihatMock::debugLines();
        $this->assertEquals('<ul><li><b>SQL</b>:SELECT * FROM users</li></ul>', str_replace(array("\r", "\n", "\t"), '', $lines));
    }

    /**
     * 测试调试信息输出，根据名称类别。
     */
    public function testDebugLinesForName() {
        SkihatMock::debug('SQL', 'SELECT * FROM users');

        $lines = SkihatMock::debugLines('SQL');
        $this->assertEquals("<ul><li><b>SQL</b>:SELECT * FROM users</li></ul>", str_replace(array("\r", "\n", "\t"), '', $lines));
    }

    /**
     * 测试调试信息输出，提供的名称参数无效。
     */
    public function testDebugLinesForNameNotSupport() {
        $lines = SkihatMock::debugLines('SQL');
        $this->assertEquals('Skihat:debug not support \'SQL\' debug lines.', $lines);
    }

    /**
     * 测试调试信息输出，写入指定文件。
     */
    public function testDebugForFile() {
        $file = SKIHAT_PATH_DATA_LOGGERS . '/run.log';
        if (is_file($file)) {
            @unlink($file);
        }

        SkihatMock::debug('SQL', 'SELECT * FROM users');
        SkihatMock::debug('SQL', 'SELECT * FROM goods');
        SkihatMock::debugLines(false, 'run.log');

        $this->assertTrue(is_file($file));
    }

    /**
     * 测试文件定位。
     */
    public function testLocate() {
        SkihatMock::register(array(SKIHAT_PATH_LOCALS_APP));

        $index = SkihatMock::locate('controllers.index_controller');
        $goods = SkihatMock::locate('controllers.goods_controller');

        $this->assertEquals(realpath($index), realpath(SKIHAT_PATH_LOCALS_APP . '/controllers/index_controller.inc'));
        $this->assertEquals(realpath($goods), realpath(SKIHAT_PATH_LOCALS_APP . '/controllers/goods_controller.inc'));
    }

    /**
     * 测试文件定位，定位失败。
     */
    public function testLocateForFail() {
        SkihatMock::register(array(SKIHAT_PATH_LOCALS_APP));

        $this->assertFalse(SkihatMock::locate('controllers.not_exists_controller'));
    }

    /**
     * 测试文件定位，优先文件。
     */
    public function testLocateForPriority() {
        SkihatMock::register(array(SKIHAT_PATH_LOCALS_APP, SKIHAT_PATH_LOCALS_LIBRARY));

        $notFound = SkihatMock::locate('controllers.not_found_controller');
        $this->assertEquals(realpath($notFound), realpath(SKIHAT_PATH_LOCALS_APP . '/controllers/not_found_controller.inc'));
    }

    /**
     * 测试文件定位，低优先级。
     */
    public function testLocateForLower() {
        SkihatMock::register(array(SKIHAT_PATH_LOCALS_APP, SKIHAT_PATH_LOCALS_LIBRARY));

        $error = SkihatMock::locate('controllers.error_controller');
        $this->assertEquals(realpath($error), realpath(SKIHAT_PATH_LOCALS_LIBRARY . '/controllers/error_controller.inc'));
    }

    /**
     * 测试文件定位，使用字符串参数。
     */
    public function testLocateForStringPaths() {
        $error = SkihatMock::locate('models.app_user', SKIHAT_PATH_LOCALS_APP);
        $this->assertEquals(realpath($error), realpath(SKIHAT_PATH_LOCALS_APP . '/models/app_user.inc'));
    }

    /**
     * 测试文件定位，使用字符串参数定位失败。
     */
    public function testLocateForStringPathsFail() {
        $this->assertFalse(SkihatMock::locate('models.item', SKIHAT_PATH_LOCALS_APP));
    }

    /**
     * 测试文件定位，使用数组参数。
     */
    public function testLocateForArrayPaths() {
        $order = SkihatMock::locate('models.app_order', array(SKIHAT_PATH_LOCALS_APP, SKIHAT_PATH_LOCALS_LIBRARY));
        $this->assertEquals(realpath($order), realpath(SKIHAT_PATH_LOCALS_APP . '/models/app_order.inc'));
    }

    /**
     * 测试文件定位，数组参数优先文件。
     */
    public function testLocateForArrayPathsPriority() {
        $order = SkihatMock::locate('models.app_order', array(SKIHAT_PATH_LOCALS_APP, SKIHAT_PATH_LOCALS_LIBRARY));
        $this->assertEquals(realpath($order), realpath(SKIHAT_PATH_LOCALS_APP . '/models/app_order.inc'));
    }

    /**
     * 测试文件定位，数组参数低优先组文件。
     */
    public function testLocateForArrayPathsLower() {
        $goods = SkihatMock::locate('models.goods', array(SKIHAT_PATH_LOCALS_APP, SKIHAT_PATH_LOCALS_LIBRARY));
        $this->assertEquals(realpath($goods), realpath(SKIHAT_PATH_LOCALS_LIBRARY . '/models/goods.inc'));
    }

    /**
     * 测试文件定位，数组参数，定位失败。
     */
    public function testLocateForArrayPathsFail() {
        $this->assertFalse(SkihatMock::locate('models.item', array(SKIHAT_PATH_LOCALS_APP, SKIHAT_PATH_LOCALS_LIBRARY)));
    }

    /**
     * 测试文件定位，使用文件扩展名。
     */
    public function testLocateForExt() {
        $mail = SkihatMock::locate('mail', SKIHAT_PATH_LOCALS_VENDOR, '.class.php');
        $this->assertEquals(realpath($mail), realpath(SKIHAT_PATH_LOCALS_VENDOR . '/mail.class.php'));
    }

    /**
     * 测试文件导入。
     */
    public function testImport() {
        SkihatMock::register(array(SKIHAT_PATH_LOCALS_APP));
        SkihatMock::import('controllers.index_controller');

        $this->assertTrue(class_exists('AppIndexController'));
    }

    /**
     * 测试文件导入，优先文件。
     */
    public function testImportForPriority() {
        SkihatMock::register(array(SKIHAT_PATH_LOCALS_APP, SKIHAT_PATH_LOCALS_LIBRARY));
        SkihatMock::import('controllers.not_found_controller');

        $this->assertTrue(class_exists('AppNotFoundController'));
    }

    /**
     * 测试文件导入，低优先文件。
     */
    public function testImportForLower() {
        SkihatMock::register(array(SKIHAT_PATH_LOCALS_APP, SKIHAT_PATH_LOCALS_LIBRARY));
        SkihatMock::import('controllers.error_controller');

        $this->assertTrue(class_exists('SkihatErrorController'));
    }

    /**
     * 测试文件导入，导入失败。
     *
     * @expectedException FileNotFoundException
     */
    public function testImportForFail() {
        SkihatMock::import('controllers.not_exists_controller');
    }

    /**
     * 测试文件导入，字符串路径。
     */
    public function testImportForStringPaths() {
        SkihatMock::import('models.app_user', SKIHAT_PATH_LOCALS_APP);
        $this->assertTrue(class_exists('AppUser'));
    }

    /**
     * 测试文件导入，字符串路径失败。
     *
     * @expectedException FileNotFoundException
     */
    public function testImportStringPathsFail() {
        SkihatMock::import('models.item', SKIHAT_PATH_LOCALS_APP);
    }

    /**
     * 测试文件导入，数组路径。
     */
    public function testImportForArrayPaths() {
        SkihatMock::import('models.app_book', array(SKIHAT_PATH_LOCALS_APP));
        $this->assertTrue(class_exists('AppBook'));
    }

    /**
     * 测试文件导入，数组路径优先级。
     */
    public function testImportForArrayPathsPriority() {
        SkihatMock::import('models.app_order', array(SKIHAT_PATH_LOCALS_APP, SKIHAT_PATH_LOCALS_LIBRARY));
        $this->assertTrue(class_exists('AppOrder'));
    }

    /**
     * 测试文件导入，数组路径低优先级。
     */
    public function testImportForArrayPathsLower() {
        SkihatMock::import('models.goods', array(SKIHAT_PATH_LOCALS_APP, SKIHAT_PATH_LOCALS_LIBRARY));
        $this->assertTrue(class_exists('SkihatGoods'));
    }

    /**
     * 测试文件导入，数组失败。
     *
     * @expectedException FileNotFoundException
     */
    public function testImportForArrayPathsFail() {
        SkihatMock::import('models.item', array(SKIHAT_PATH_LOCALS_APP, SKIHAT_PATH_LOCALS_VENDOR));
    }

    /**
     * 测试文件导入，重复导入。
     */
    public function testImportForRepeat() {
        SkihatMock::import('controllers.book_controller', SKIHAT_PATH_LOCALS_APP);
        SkihatMock::import('controllers.book_controller', SKIHAT_PATH_LOCALS_LIBRARY);

        $this->assertTrue(class_exists('AppBookController'));
        $this->assertFalse(class_exists('SkihatBookController'));
    }

    /**
     * 测试文件导入，重复导入包含扩展名。
     */
    public function testImportForRepeatAndExt() {
        SkihatMock::import('mail', SKIHAT_PATH_LOCALS_VENDOR, '.class.php');
        SkihatMock::import('mail', SKIHAT_PATH_LOCALS_APP);

        $this->assertTrue(class_exists('AppMail'));
        $this->assertTrue(class_exists('VendorMail'));
    }

    /**
     * 测试文件导入，定义返回值。
     */
    public function testImportForDeclareReturn() {
        $default = SkihatMock::import('langs.default', SKIHAT_PATH_LOCALS_APP);
        $array = SkihatMock::import('langs.array', SKIHAT_PATH_LOCALS_APP);

        $this->assertEquals(1, $default);
        $this->assertTrue(is_array($array));
    }

    /**
     * 测试类导入。
     */
    public function testUsing() {
        SkihatMock::register(array(SKIHAT_PATH_LOCALS_APP));

        $this->assertEquals('OrderService', Skihat::using('services.order_service'));
        $this->assertTrue(class_exists('OrderService'));
    }

    /**
     * 测试类型导入，包含文件和类名称。
     */
    public function testUsingForFileAndType() {
        SkihatMock::register(array(SKIHAT_PATH_LOCALS_APP));

        $this->assertEquals('AppGoodsService', Skihat::using('services.goods_service#AppGoodsService'));
        $this->assertTrue(class_exists('AppGoodsService'));
    }

    /**
     * 测试类型导入，已存在的类型。
     */
    public function testUsingForExistsType() {
        SkihatMock::import('services.book_service', SKIHAT_PATH_LOCALS_APP);

        $this->assertTrue(class_exists('BookService'));
        $this->assertEquals('BookService', Skihat::using('#BookService'));
    }

    /**
     * 测试类型导入，不包含包。
     */
    public function testUsingForEmptyPackage() {
        $this->assertEquals('Pdf', SkihatMock::using('pdf', SKIHAT_PATH_LOCALS_APP));
        $this->assertEquals('Pdf', SkihatMock::using('pdf#Pdf', SKIHAT_PATH_LOCALS_APP));
    }

    /**
     * 测试类型导入，导入文件失败。
     *
     * @expectedException FileNotFoundException
     */
    public function testUsingForFileFail() {
        SkihatMock::using('services.user_service');
    }

    /**
     * 测试类型导入，类型失败。
     *
     * @expectedException TypeNotFoundException
     */
    public function testUsingForTypeFail() {
        SkihatMock::register(array(SKIHAT_PATH_LOCALS_APP));
        SkihatMock::using('services.archive_service');
    }

    /**
     * 测试Ioc创建实例，使用字符串类型。
     */
    public function testIocForStringClass() {
        SkihatMock::register(array(SKIHAT_PATH_LOCALS_APP));

        $module = SkihatMock::ioc('models.app_book');
        $this->assertInstanceOf('AppBook', $module);
    }

    /**
     * 测试Ioc创建实例，使用数组参数。
     */
    public function testIocForArrayClass() {
        SkihatMock::register(array(SKIHAT_PATH_LOCALS_APP));

        $module = SkihatMock::ioc(array(SKIHAT_IOC_CLASS => 'models.app_order'));
        $this->assertInstanceOf('AppOrder', $module);
    }

    /**
     * 测试Ioc创建实例，SKIHAT_IOC_CLASS没有值。
     *
     * @expectedException ConfigException
     */
    public function testIocForArrayClassNotSupport() {
        SkihatMock::ioc(array());
    }

    /**
     * 测试Ioc创建实例，SKIHAT_IOC_PATHS配置参数。
     */
    public function testIocForArrayPaths() {
        $model = SkihatMock::ioc(array(
            SKIHAT_IOC_CLASS => 'models.app_user',
            SKIHAT_IOC_PATHS => SKIHAT_PATH_LOCALS_APP));

        $this->assertInstanceOf('AppUser', $model);
    }

    /**
     * 测试Ioc创建实例，SKIHAT_IOC_PATHS与$ext参数冲突。
     */
    public function testIocForArrayPathsAndPathsArgs() {
        $pdf = SkihatMock::ioc(array(
            SKIHAT_IOC_CLASS => 'pdf',
            SKIHAT_IOC_PATHS => SKIHAT_PATH_LOCALS_APP
        ), SKIHAT_PATH_LOCALS_VENDOR);

        $this->assertEquals('app', $pdf->path());
    }

    /**
     * 测试Ioc创建实例，SKIHAT_IOC_EXT配置参数。
     */
    public function testIocForArrayExt() {
        SkihatMock::register(array(SKIHAT_PATH_LOCALS_VENDOR));

        $mail = SkihatMock::ioc(array(
            SKIHAT_IOC_CLASS => 'mail#VendorMail',
            SKIHAT_IOC_EXT => '.class.php'
        ));

        $this->assertInstanceOf('VendorMail', $mail);
    }

    /**
     * 测试Ioc创建实例，SKIHAT_IOC_INITIAL
     */
    public function testIocForInitial() {
        $obj = SkihatMock::ioc(array(
            SKIHAT_IOC_CLASS => '#InitialMock',
            SKIHAT_IOC_INITIAL => 'initialize'
        ));

        $this->assertTrue($obj->initialize);
    }

    /**
     * 测试Ioc创建实例，使用属性值。
     */
    public function testIocForProps() {
        $obj = SkihatMock::ioc(array(
            SKIHAT_IOC_CLASS => '#InitialMock',
            'hello' => 'skihat'
        ));

        $this->assertEquals('skihat', $obj->hello);
    }

    /**
     * 测试i18n
     */
    public function testI18n() {
        $name = SkihatMock::i18n('default.name');
        $this->assertEquals('skihat', $name);
    }

    /**
     * 测试i18n，使用默认值。
     */
    public function testI18nForDefault() {
        $default = SkihatMock::i18n('default.empty', 'default');
        $defaultVal = SkihatMock::i18n('default.empty');

        $this->assertEquals('default', $default);
        $this->assertEquals('', $defaultVal);
    }

    /**
     * 测试i18n，指定语言。
     */
    public function testI18nForLanguage() {
        $this->assertEquals('订单', SkihatMock::i18n('order.title', 'order', 'zh-CN'));
        $this->assertEquals('submit order', SkihatMock::i18n('order.title', 'order', 'en-US'));
    }


    /**
     * 测试i18n,指定模块信息。
     */
    public function testI18nForModule() {
        $this->assertEquals('Hello Core', SkihatMock::i18n('core:default.name'));
        $this->assertEquals('Hello Skihat', SkihatMock::i18n('core:default.title'));
    }

    /**
     * 测试i18n，使用默认参数值。
     */
    public function testI18nForModuleDefault() {
        $this->assertEquals('default', SkihatMock::i18n('core:default.order', 'default'));
        $this->assertEquals('', SkihatMock::i18n('core:default.order'));
    }

    /**
     * 测试i18n，使用模块语言参数。
     */
    public function testI18nForModuleLang() {
        $this->assertEquals('Submit Core', SkihatMock::i18n('core:goods.title', '', 'en-US'));
        $this->assertEquals('提交Core', SkihatMock::i18n('core:goods.title', '', 'zh-CN'));
    }
}

/**
 * Skihat 模拟类，提供针对单元测试的模拟要求。
 *
 * @package skihat.kernels
 */
class SkihatMock extends Skihat {
    public static function reset() {
        self::$_conf = array();
        self::$_paths = array();
        self::$_langs = array();
        self::$_debugs = array();
        self::$_debugLines = array();
    }

    public static function paths() {
        return self::$_paths;
    }

    public static function debugs() {
        return self::$_debugs;
    }
}

/**
 * 初始化模拟对象，提供模拟的初始化服务。
 *
 * @package skihat.kernels
 */
class InitialMock {
    public $initialize = false;
    public $hell = 'world';

    function initialize() {
        $this->initialize = true;
    }
}