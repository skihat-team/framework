<?php

/**
 * xcache_engine.inc 提供Xcache缓存引擎声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.caches.engines
 * @version   1.0
 *
 * @created   2012-07-16
 * @modified  2013-08-06
 */

/**
 * Xcache缓存引擎，提供Xcache的缓存解决方案。
 *
 * @package skihat.kernels.caches.engines
 */
class XcacheEngine implements ICacheEngine {

    /**
     * @see ICacheEngine::write
     */
    public function write($name, $value, $expire = Cache::DEFAULT_EXPIRE) {
        return xcache_set($name, $value, $expire);
    }

    /**
     * @see ICacheEngine::read
     */
    public function read($name) {
        return xcache_get($name);
    }

    /**
     * @see ICacheEngine::delete
     */
    public function delete($name) {
        return xcache_unset($name);
    }

    /**
     * @see ICacheEngine::flush
     */
    public function flush() {
        xcache_clear_cache(XC_TYPE_VAR, 0);
        return true;
    }
}
