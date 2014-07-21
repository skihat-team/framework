<?php
/**
 * apc_cache_engine.inc 提供APC缓存引擎声明。
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
 * APC 缓存引擎，提供APC缓存解决方案。
 *
 * @package skihat.kernels.caches.engines
 */
class ApcCacheEngine implements ICacheEngine {

    /**
     * @see ICacheEngine::write
     */
    public function write($name, $value, $expire = Cache::DEFAULT_EXPIRE) {
        return apc_store($name, $value, $expire);
    }

    /**
     * @see ICacheEngine::read
     */
    public function read($name) {
        return apc_fetch($name);
    }

    /**
     * @see ICacheEngine::delete
     */
    public function delete($name) {
        return apc_delete($name);
    }

    /**
     * @see ICacheEngine::flush
     */
    public function flush() {
        return apc_clear_cache();
    }
}