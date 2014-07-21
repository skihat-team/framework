<?php
/**
 * memcache_engine.inc 提供Memcache缓存引擎声明。
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

/**
 * Memcache 缓存引擎类，提供Memcache缓存引擎解决方案。
 *
 * @package skihat.kernels.caches.engines
 */
class MemcacheEngine implements ICacheEngine {

    /**
     * @var string 主机服务器地址，默认为127.0.0.1。
     */
    public $host = '127.0.0.1';

    /**
     * @var int 主机端口地址，默认为11211。
     */
    public $port = 11211;

    /**
     * @var int 缓存标记。
     */
    public $flag = MEMCACHE_COMPRESSED;

    /**
     * 初始化方法，使用Ioc创建时必须使用SKIHAT_IOC_INITIAL执行initialize方法。
     */
    public function initialize() {
        $this->_memcache = new Memcache();
        if (!$this->_memcache->connect($this->host, $this->port, $this->flag)) {
            throw new CacheException('MemcacheEngine:initialize fail.');
        }
    }

    /**
     * @see ICacheEngine::write
     */
    public function write($name, $value, $expire = Cache::DEFAULT_EXPIRE) {
        return $this->_memcache->set($name, $value, $this->flag, $expire);
    }

    /**
     * @see ICacheEngine::read
     */
    public function read($name) {
        return $this->_memcache->get($name);
    }

    /**
     * @see ICacheEngine::delete
     */
    public function delete($name) {
        return $this->_memcache->delete($name);
    }

    /**
     * @see ICacheEngine::flush
     */
    public function flush() {
        return $this->_memcache->flush();
    }

    /**
     * 析构方法，自动关闭缓存实例。
     */
    public function __destruct() {
        if ($this->_memcache) {
            $this->_memcache->close();
        }
    }

    /**
     * @var bool|Memcache 缓存处理实例。
     */
    protected $_memcache = false;
}