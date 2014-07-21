<?php
/**
 * file_cache_engine_test.inc 提供文件缓存引擎单元测试声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.caches.engines
 * @version   1.0
 *
 * @created   2012-07-16
 * @modified  2013-08-05
 */

# 开启单元测试启动服务。
require_once(dirname(dirname(dirname(dirname(dirname(__FILE__))))) . '/startup.inc');

# 引用被测试文件。
Skihat::import('kernels.caches.cache', SKIHAT_PATH_LIBRARY);
Skihat::import('kernels.caches.engines.file_cache_engine', SKIHAT_PATH_LIBRARY);

/**
 * 文件缓存引擎单元测试，提供文件缓存引擎的单元测试服务。
 *
 * @package skihat.kernels.caches.engines
 */
class FileCacheEngineTest extends PHPUnit_Framework_TestCase {
    /**
     * 测试写读字符串缓存值。
     */
    public function testWriteReadForString() {
        $engine = Skihat::ioc('#FileCacheEngine');

        $engine->write('mysql', 'hello mysql');
        $engine->write('oracle', 'hello oracle');

        $this->assertFileExists(SKIHAT_PATH_DATA_CACHES . '/default-cache-mysql.tmp');
        $this->assertFileExists(SKIHAT_PATH_DATA_CACHES . '/default-cache-oracle.tmp');

        $this->assertEquals('hello mysql', $engine->read('mysql'));
        $this->assertEquals('hello oracle', $engine->read('oracle'));
    }

    /**
     * 测试读写数组缓存值。
     */
    public function testWriteReadForArray() {
        $engine = Skihat::ioc('#FileCacheEngine');

        $engine->write('mysql', array('host' => '127.0.0.1', 'pass' => '123321'));
        $engine->write('oracle', array('host' => 'localhost', 'pass' => '132'));

        $this->assertFileExists(SKIHAT_PATH_DATA_CACHES . '/default-cache-mysql.tmp');
        $this->assertFileExists(SKIHAT_PATH_DATA_CACHES . '/default-cache-oracle.tmp');

        $mysql = $engine->read('mysql');
        $oracle = $engine->read('oracle');

        $this->assertEquals('127.0.0.1', $mysql['host']);
        $this->assertEquals('123321', $mysql['pass']);

        $this->assertEquals('localhost', $oracle['host']);
        $this->assertEquals('132', $oracle['pass']);
    }

    /**
     * 测试读取会员缓存值。
     */
    public function testWriteReadForEmployee() {
        $engine = Skihat::ioc('#FileCacheEngine');

        $engine->write('mysql', new Employee());
        $engine->write('oracle', new Employee('emp-02', 'name-02'));

        $this->assertFileExists(SKIHAT_PATH_DATA_CACHES . '/default-cache-mysql.tmp');
        $this->assertFileExists(SKIHAT_PATH_DATA_CACHES . '/default-cache-oracle.tmp');

        $mysql = $engine->read('mysql');
        $oracle = $engine->read('oracle');

        $this->assertEquals('emp-01', $mysql->id);
        $this->assertEquals('name-01', $mysql->name);

        $this->assertEquals('emp-02', $oracle->id);
        $this->assertEquals('name-02', $oracle->name);
    }

    /**
     * 测试写入，读取缓存值，指定前缀。
     */
    public function testWriteReadForPrefix() {
        $mysqlFile = SKIHAT_PATH_DATA_CACHES . '/runtime-cache-mysql.tmp';
        $oracleFile = SKIHAT_PATH_DATA_CACHES . '/runtime-cache-oracle.tmp';

        if (is_file($mysqlFile)) @unlink($mysqlFile);
        if (is_file($oracleFile)) @unlink($oracleFile);

        $engine = Skihat::ioc('#FileCacheEngine');

        $engine->prefix = 'runtime-cache';
        $engine->write('mysql', 'hello mysql');
        $engine->write('oracle', 'hello oracle');

        $this->assertFileExists($mysqlFile);
        $this->assertFileExists($oracleFile);

        $this->assertEquals('hello mysql', $engine->read('mysql'));
        $this->assertEquals('hello oracle', $engine->read('oracle'));
    }

    /**
     * 测试写入，读取值，指定路径。
     */
    public function testWriteReadForPaths() {
        $mysqlFile = SKIHAT_PATH_DATA_CACHES . '/runtime/default-cache-mysql.tmp';
        if (is_file($mysqlFile)) @unlink($mysqlFile);

        $engine = Skihat::ioc('#FileCacheEngine');
        $engine->paths = SKIHAT_PATH_DATA_CACHES . '/runtime';
        $engine->write('mysql', 'hello mysql');

        $this->assertFileExists($mysqlFile);
        $this->assertEquals('hello mysql', $engine->read('mysql'));
    }

    /**
     * 测试读取缓存值，缓存过期。
     */
    public function testReadForExpire() {
        $engine = Skihat::ioc('#FileCacheEngine');
        $engine->write('mysql', 'mysql value', -100);

        $this->assertFileExists(SKIHAT_PATH_DATA_CACHES . '/default-cache-mysql.tmp');
        $this->assertFalse($engine->read('mysql'));
    }

    /**
     * 测试读取缓存值，不存在的值。
     */
    public function testReadForNotExists() {
        $engine = Skihat::ioc('#FileCacheEngine');

        $this->assertFalse($engine->read('not-exists'));
    }

    /**
     * 测试删除缓存值。
     */
    public function testDelete() {
        $engine = Skihat::ioc('#FileCacheEngine');
        $engine->write('mysql', 'hello mysql');

        $this->assertEquals('hello mysql', $engine->read('mysql'));

        $engine->delete('mysql');
        $this->assertFileNotExists(SKIHAT_PATH_DATA_CACHES . '/default-cache-mysql.tmp');
        $this->assertFalse($engine->read('mysql'));
    }

    /**
     * 测试删除缓存值，值还未写入。
     */
    public function testDeleteForNotWrite() {
        $engine = Skihat::ioc('#FileCacheEngine');

        $this->assertTrue($engine->delete('mysql'));
    }

    /**
     * 测试清空缓存值。
     */
    public function testFlush() {
        $engine = Skihat::ioc('#FileCacheEngine');
        $engine->write('mysql', 'hello mysql');
        $engine->flush();

        $this->assertFalse($engine->read('mysql'));
        $this->assertFileExists(SKIHAT_PATH_DATA_CACHES . '/custom-cache-mysql.tmp');
    }

    public function setUp() {
        if (is_file(SKIHAT_PATH_DATA_CACHES . '/default-cache-mysql.tmp')) {
            @unlink(SKIHAT_PATH_DATA_CACHES . '/default-cache-mysql.tmp');
        }
        if (is_file(SKIHAT_PATH_DATA_CACHES . '/default-cache-oracle.tmp')) {
            @unlink(SKIHAT_PATH_DATA_CACHES . '/default-cache-oracle.tmp');
        }
    }
}

/**
 * 文件缓存员工类，提供缓存测试服务支持。
 *
 * @package skihat.kernals.caches.engines
 */
class Employee {
    public $id = false;
    public $name = false;

    public function __construct($id = 'emp-01', $name = 'name-01') {
        $this->id = $id;
        $this->name = $name;
    }
}