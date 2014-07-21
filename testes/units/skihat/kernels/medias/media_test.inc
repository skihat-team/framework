<?php
/**
 * media_test.inc 提供网站内部资源管理接口声明单元测试声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.meidas
 * @version   1.0
 *
 * @created   2012-07-12
 * @modified  2013-08-06
 */

# 引入测试启动文件
require_once(dirname(dirname(dirname(dirname(__FILE__)))) . '/startup.inc');

# 导入被测试资源接口。
Skihat::import('kernels.medias.media', SKIHAT_PATH_LIBRARY);

/**
 * 资源接口单元测试类，提供对资源接口类的单元测试服务。
 *
 * @package skihat.kernels.medias
 */
class MediaTest extends PHPUnit_Framework_TestCase {

    /**
     * 测试上传，使用默认组件。
     */
    public function testUploadForDefault() {
        Media::upload('hello');

        $files = DefaultMediaEngine::$files;

        $this->assertEquals(1, count($files));
        $this->assertEquals('hello', $files[0]);
    }

    /**
     * 测试上传，使用实例组合。
     */
    public function testUploadForExample() {
        Media::upload('hello', false, 'example');

        $files = ExampleMediaEngine::$files;

        $this->assertEquals(1, count($files));
        $this->assertEquals('hello', $files[0]);
    }

    /**
     * 测试上传，使用无效引擎。
     *
     * @expectedException ConfigException
     */
    public function testUploadForInvalidEngine() {
        Media::upload(false, false, 'invalid');
    }

    /**
     * 测试获取文件列表，使用默认引擎。
     */
    public function testFilesForDefault() {
        DefaultMediaEngine::$files = array('hello');

        $files = Media::files();

        $this->assertEquals(1, count($files));
        $this->assertEquals('hello', $files[0]);
    }

    /**
     * 测试获取文件列表，使用实例引擎。
     */
    public function testFilesForExample() {
        ExampleMediaEngine::$files = array('hello');

        $files = Media::files(false, false, 'example');

        $this->assertEquals(1, count($files));
        $this->assertEquals('hello', $files[0]);
    }

    /**
     * 测试获取文件列表，使用无效引擎。
     *
     * @expectedException ConfigException
     */
    public function testFilesForInvalidEngine() {
        Media::files(false, false, 'invalid');
    }

    /**
     * @see PHPUnit_Framework_TestCase::setUp
     */
    public function setUp() {
        Skihat::write(Media::CONFIG_NAME, array(
            Media::DEFAULT_ENGINE => '#DefaultMediaEngine',
            'example' => '#ExampleMediaEngine'
        ));

        Media::initialize();

        DefaultMediaEngine::$files = array();
        ExampleMediaEngine::$files = array();
    }
}

/**
 * 默认资源引擎，提供资源接口测试的支持。
 *
 * @package skihat.kernels.meidas
 */
class DefaultMediaEngine implements IMediaEngine {
    public static $files = array();

    /**
     * @see IMediaEngine::files
     */
    public function files($path = false, $sort = false) {
        return self::$files;
    }

    /**
     * @see IMediaEngine::upload
     */
    public function upload($file, $path = false) {
        array_push(self::$files, $file);
    }
}

/**
 * 实例资源引擎，提供资源接口测试的支持。
 */
class ExampleMediaEngine implements IMediaEngine {
    public static $files = array();

    /**
     * @see IMediaEngine::files
     */
    public function files($path = false, $sort = false) {
        return self::$files;
    }

    /**
     * @see IMediaEngine::upload
     */
    public function upload($file, $path = false) {
        array_push(self::$files, $file);
    }
}

