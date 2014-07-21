<?php
/**
 * cache_test.inc 提供内核缓存组件接口测试声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.caches
 * @version   1.0
 *
 * @created   2012-07-16
 * @modified  2013-08-05
 */

# 单元测试启动服务支持。
require_once(dirname(dirname(dirname(dirname(__FILE__)))) . '/startup.inc');

# 缓存组件。
Skihat::import('kernels.caches.cache', SKIHAT_PATH_LIBRARY);

/**
 * 测试接口声明，提供缓存组件的单元测试服务。
 *
 * @package skihat.kernels.caches
 */
class CacheTest extends PHPUnit_Framework_TestCase {

    /**
     * 测试写入缓存。
     */
    public function testWrite() {
        Cache::write('name', 'hello skihat');
        Cache::write('name', 'hello framework', Cache::RUNTIME_ENGINE);

        $this->assertEquals('hello skihat', DefaultCacheEngine::$caches['name']);
        $this->assertEquals('hello framework', RuntimeCacheEngine::$caches['name']);
    }

    /**
     * 测试写入缓存，无效的引擎。
     *
     * @expectedException ConfigException
     */
    public function testWriteForInvalidEngine() {
        Cache::write('name', 'hello', 'invalid');
    }

    /**
     * 测试读取缓存。
     */
    public function testRead() {
        DefaultCacheEngine::$caches = array('name' => 'hello skihat');
        RuntimeCacheEngine::$caches = array('name' => 'hello framework');

        $this->assertEquals('hello skihat', Cache::read('name'));
        $this->assertEquals('hello framework', Cache::read('name', Cache::RUNTIME_ENGINE));
    }

    /**
     * 测试读取缓存，名称不存在。
     */
    public function testReadForNameNotExists() {
        $this->assertFalse(Cache::read('name'));
        $this->assertFalse(Cache::read('name', Cache::RUNTIME_ENGINE));
    }

    /**
     * 测试读取缓存，无效的引擎。
     *
     * @expectedException ConfigException
     */
    public function testReadForInvalidEngine() {
        Cache::read('name', 'invalid');
    }

    /**
     * 测试删除缓存。
     */
    public function testDelete() {
        DefaultCacheEngine::$caches = array('name' => 'hello skihat');
        RuntimeCacheEngine::$caches = array('name' => 'hello framework');

        Cache::delete('name');
        Cache::delete('name', Cache::RUNTIME_ENGINE);

        $this->assertEmpty(DefaultCacheEngine::$caches);
        $this->assertEmpty(RuntimeCacheEngine::$caches);
    }

    /**
     * 测试删除缓存值，无效引擎。
     *
     * @expectedException ConfigException
     */
    public function testDeleteForInvalidEngine() {
        Cache::delete('name', 'invalid');
    }

    /**
     * 测试清空缓存值。
     */
    public function testFlush() {
        DefaultCacheEngine::$caches = array('name' => 'hello skihat');
        RuntimeCacheEngine::$caches = array('name' => 'hello framework');

        Cache::flush();
        Cache::flush(Cache::RUNTIME_ENGINE);

        $this->assertEmpty(DefaultCacheEngine::$caches);
        $this->assertEmpty(RuntimeCacheEngine::$caches);
    }

    /**
     * 测试清空缓存值，无效引擎。
     *
     * @expectedException ConfigException
     */
    public function testFlushForInvalidEngine() {
        Cache::flush('invalid');
    }

    public function setUp() {
        Skihat::write(Cache::CONFIG_NAME, array(
            Cache::DEFAULT_ENGINE => '#DefaultCacheEngine',
            Cache::RUNTIME_ENGINE => '#RuntimeCacheEngine'
        ));

        Cache::initialize();

        DefaultCacheEngine::$caches = array();
        RuntimeCacheEngine::$caches = array();
    }
}

/**
 * 默认缓存组件，提供缓存的单元测试模拟服务。
 *
 * @package skihat.kernels.caches
 */
class DefaultCacheEngine implements ICacheEngine {
    public static $caches = array();

    public function write($name, $value, $expire = Cache::DEFAULT_EXPIRE) {
        self::$caches[$name] = $value;
        return true;
    }

    public function read($name) {
        return array_key_value(self::$caches, $name, false);
    }

    public function delete($name) {
        unset(self::$caches[$name]);
        return true;
    }

    public function flush() {
        self::$caches = array();
    }
}

/**
 * 运行时缓存组件，提供缓存的单元测试模拟服务。
 *
 * @package skihat.kernels.caches
 */
class RuntimeCacheEngine implements ICacheEngine {
    public static $caches = array();

    public function write($name, $value, $expire = Cache::DEFAULT_EXPIRE) {
        self::$caches[$name] = $value;
        return true;
    }

    public function read($name) {
        return array_key_value(self::$caches, $name, false);
    }

    public function delete($name) {
        unset(self::$caches[$name]);
        return true;
    }

    public function flush() {
        self::$caches = array();
    }
}