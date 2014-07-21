<?php
/**
 * file_media_engine_test.inc 提供服务器文件资源管理引擎单元测试声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.medias.engines
 * @version   1.0
 *
 * @created   2012-07-12
 * @modified  2013-08-06
 */

# 引用测试启动文件
require_once(dirname(dirname(dirname(dirname(dirname(__FILE__))))) . '/startup.inc');

# 引用被测试文件
Skihat::import('kernels.medias.media', SKIHAT_PATH_LIBRARY);
Skihat::import('kernels.medias.engines.file_media_engine', SKIHAT_PATH_LIBRARY);

define('UPLOAD_PATH', SKIHAT_PATH_APP_PUBLICS_UPLOADS . '/boy');

/**
 * 文件资源引擎单元测试类，提供文件资源引擎的单元测试支持。
 *
 * @package skihat.kernels.medias.engines
 */
class FileMediaEngineTest extends PHPUnit_Framework_TestCase {
    /**
     * 测试获取文件信息。
     */
    public function testFiles() {
        $engine = Skihat::ioc('#FileMediaEngine');
        $files = $engine->files();

        $this->assertEquals(4, count($files));
        $this->assertEquals('boy', $files[0][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('girls', $files[1][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('car.jpg', $files[2][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('sea.jpg', $files[3][IMediaEngine::SCHEMA_FILE_NAME]);
    }

    /**
     * 测试获取文件信息，指定路径值。
     */
    public function testFilesForPath() {
        $engine = Skihat::ioc('#FileMediaEngine');
        $files = $engine->files('girls');

        $this->assertEquals(2, count($files));
        $this->assertEquals('more', $files[0][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('02.jpg', $files[1][IMediaEngine::SCHEMA_FILE_NAME]);
    }

    /**
     * 测试获取文件信息，指定更多路径值。
     */
    public function testFilesForPathMore() {
        $engine = Skihat::ioc('#FileMediaEngine');
        $files = $engine->files('girls/more');

        $this->assertEquals(2, count($files));
        $this->assertEquals('car.jpg', $files[0][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('sea.jpg', $files[1][IMediaEngine::SCHEMA_FILE_NAME]);
    }

    /**
     * 测试读取文件信息，指定路径安全。
     */
    public function testFilesForPathSafe() {
        $engine = Skihat::ioc('#FileMediaEngine');
        $files = $engine->files('../..');

        $this->assertEmpty($files);
    }

    /**
     * 测试获取文件信息，允许查看的值。
     */
    public function testFilesForAllowViews() {
        $engine = Skihat::ioc(array(SKIHAT_IOC_CLASS => '#FileMediaEngine', 'allowViews' => array('txt', 'jpg')));
        $files = $engine->files('girls');

        $this->assertEquals(3, count($files));
        $this->assertEquals('more', $files[0][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('01.txt', $files[1][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('02.jpg', $files[2][IMediaEngine::SCHEMA_FILE_NAME]);
    }

    /**
     * 测试获取文件信息，指定排序方式。
     */
    public function testFilesForSortSize() {
        $engine = Skihat::ioc(array(SKIHAT_IOC_CLASS => '#FileMediaEngine', 'allowViews' => array('txt', 'jpg')));
        $files = $engine->files('girls', 'size');

        $this->assertEquals(3, count($files));
        $this->assertEquals('more', $files[0][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('02.jpg', $files[1][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('01.txt', $files[2][IMediaEngine::SCHEMA_FILE_NAME]);
    }

    /**
     * 测试读取文件信息，指定类型排序。
     */
    public function testFilesForSortType() {
        $engine = Skihat::ioc(array(SKIHAT_IOC_CLASS => '#FileMediaEngine', 'allowViews' => array('jpg', 'ask', 'txt')));
        $files = $engine->files('girls', 'type');

        $this->assertEquals(4, count($files));
        $this->assertEquals('more', $files[0][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('03.ask', $files[1][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('02.jpg', $files[2][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('01.txt', $files[3][IMediaEngine::SCHEMA_FILE_NAME]);
    }

    /**
     * 测试读取文件信息，按创建时间排列。
     */
    public function testFilesForSortDatetime() {
        $engine = Skihat::ioc(array(SKIHAT_IOC_CLASS => '#FileMediaEngine', 'allowViews' => array('jpg', 'ask', 'txt')));
        $files = $engine->files('girls', 'datetime');

        $this->assertEquals(4, count($files));
        $this->assertEquals('more', $files[0][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('02.jpg', $files[1][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('01.txt', $files[2][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('03.ask', $files[3][IMediaEngine::SCHEMA_FILE_NAME]);
    }

    /**
     * 测试读取文件信息，指定主机路径。
     */
    public function testFilesForHostPath() {
        $engine = Skihat::ioc(array(
            SKIHAT_IOC_CLASS => '#FileMediaEngine',
            'allowViews' => array('php'),
            'hostPath' => SKIHAT_PATH_APP_PUBLICS));

        $files = $engine->files();

        $this->assertEquals(2, count($files));
        $this->assertEquals('uploads', $files[0][IMediaEngine::SCHEMA_FILE_NAME]);
        $this->assertEquals('index.php', $files[1][IMediaEngine::SCHEMA_FILE_NAME]);
    }

    /**
     * 测试上传文件。
     */
    public function testUpload() {
        $engine = Skihat::ioc(array(SKIHAT_IOC_CLASS => '#FileMediaEngineMock', 'hostPath' => UPLOAD_PATH));
        $url = $engine->upload(array('name' => 'cart.jpg', 'type' => 'image/jpeg', 'error' => 0,
            'size' => 1222, 'tmp_name' => SKIHAT_PATH_APP_PUBLICS_UPLOADS . '/car.jpg'));

        $this->assertEquals('/uploads/car.jpg', $url);
        $this->assertFileExists(SKIHAT_PATH_APP_PUBLICS_UPLOADS . '/boy/car.jpg');
    }

    /**
     * 测试上传文件，指定允许上传的类型。
     */
    public function testUploadForAllowUploadTypes() {
        $engine = Skihat::ioc(array(SKIHAT_IOC_CLASS => '#FileMediaEngineMock', 'hostPath' => UPLOAD_PATH,
            'allowUploadTypes' => array('txt' => 'text/plan')));
        $url = $engine->upload(array('name' => '01.txt', 'type' => 'text/plan', 'error' => 0,
            'size' => 1122,
            'tmp_name' => SKIHAT_PATH_APP_PUBLICS_UPLOADS . '/girls/01.txt'));

        $this->assertEquals('/uploads/car.txt', $url);
        $this->assertFileExists(SKIHAT_PATH_APP_PUBLICS_UPLOADS . '/boy/car.txt');
    }

    /**
     * 测试上传文件，指定上传类型，失败。
     */
    public function testUploadForAllowUploadTypesFail() {
        $engine = Skihat::ioc('#FileMediaEngineMock');

        $this->assertFalse($engine->upload(array('name' => '01.txt', 'error' => 0, 'size' => 122)));
        $this->assertFalse($engine->upload(array('name' => 'car.jpg', 'error' => 0, 'size' => 112, 'type' => 'text/html')));
        $this->assertFalse($engine->upload(array('name' => '01.txt', 'error' => 0, 'size' => 112, 'type' => 'image/gif')));
    }

    /**
     * 测试上传文件，上传包含错误。
     */
    public function testUploadForErrors() {
        $engine = Skihat::ioc("#FileMediaEngineMock");

        $this->assertFalse($engine->upload(array('name' => 'car.jpg', 'error' => 1)));
    }

    /**
     * 测试上传文件，大于指定大小。
     */
    public function testUploadForSizeFail() {
        $engine = Skihat::ioc('#FileMediaEngineMock');

        $this->assertFalse($engine->upload(array('name' => 'car.jpg', 'error' => 0, 'size' => 1092912)));
    }

    /**
     * 测试上传文件，指定路径参数。
     */
    public function testUploadForPath() {
        $engine = Skihat::ioc('#FileMediaEngineMock');
        $url = $engine->upload(array('name' => 'car.jpg', 'type' => 'image/jpeg', 'error' => 0,
            'size' => 1122, 'tmp_name' => SKIHAT_PATH_APP_PUBLICS_UPLOADS . '/car.jpg'), 'boy');

        $this->assertEquals('/uploads/boy/car.jpg', $url);
        $this->assertFileExists(UPLOAD_PATH . '/car.jpg');
    }

    /**
     * 测试上传文件，指定路径安全。
     */
    public function testUploadForPathSafe() {
        $engine = Skihat::ioc('#FileMediaEngineMock');
        $url = $engine->upload(array('name' => 'car.jpg', 'type' => 'image/jpeg', 'error' => 0,
            'size' => 1122, 'tmp_name' => SKIHAT_PATH_APP_PUBLICS_UPLOADS . '/car.jpg'), '../..');

        $this->assertFalse($url);
    }

    /**
     * @see PHPUnit_Framework_TestCase::setup
     */
    public function setUp() {
        @unlink(UPLOAD_PATH . '/car.jpg');
        @unlink(UPLOAD_PATH . '/car.txt');
    }
}

/**
 * 文件资源引擎模拟对象，提供文件上传模拟服务支持。
 *
 * @package skihat.kernels.medias.engines
 */
class FileMediaEngineMock extends FileMediaEngine {

    /**
     * @see FileMediaEngine::_uploadFullName
     */
    protected function _uploadFullName($path, $file) {
        return $path . '/car.' . pathinfo($file['name'], PATHINFO_EXTENSION);
    }

    /**
     * @see FileMediaEngine::_uploadStroe
     */
    protected function _uploadStore($file, $fullName) {
        return copy($file['tmp_name'], $fullName);
    }
}