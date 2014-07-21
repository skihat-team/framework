<?php
/**
 * cache.inc 提供内核缓存组件接口声明。
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

/**
 * 缓存组件异常，当缓存发生错误时引发本异常。
 *
 * @package skihat.kernels.caches
 */
class CacheException extends SkihatException {
}


/**
 * 缓存引擎接口，提供不同缓存解决方案。
 *
 * @package skihat.kernels.caches
 */
interface ICacheEngine {

    /**
     * 根据$name,$value和$expire写入缓存值，如果$name值已存在则覆盖原有值。
     *
     * @param string $name   缓存名称。
     * @param mixed  $value  缓存值。
     * @param int    $expire 过期时间（单位秒）。
     *
     * @return mixed
     */
    function write($name, $value, $expire = Cache::DEFAULT_EXPIRE);

    /**
     * 根据$name参数读取缓存的值，如果值不存在或已过期返回false。
     *
     * @param string $name 缓存名称。
     *
     * @return mixed|bool
     */
    function read($name);

    /**
     * 根据$name参数删除缓存的值，并返回结果。
     *
     * @param string $name 缓存名称。
     *
     * @return mixed
     */
    function delete($name);

    /**
     * 清空缓存引擎的全部缓存值，并返回结果。
     *
     * @return mixed
     */
    function flush();
}


/**
 * 缓存组件服务口，提供缓存组合件的外部访问接口。
 *
 * @package skihat.kernels.caches
 */
abstract class Cache {
    /**
     * @const 错误调试名称。
     */
    const DEBUG_NAME = 'kernels/caches';

    /**
     * @const 默认过期时间（单位秒）。
     */
    const DEFAULT_EXPIRE = 1800;

    /**
     * @const 默认引擎名称。
     */
    const DEFAULT_ENGINE = 'default';

    /**
     * @const string 运行时缓存引擎。
     */
    const RUNTIME_ENGINE = 'runtime';

    /**
     * @const string 缓存名称。
     */
    const CONFIG_NAME = 'kernels/caches';

    /**
     * 初始化组合。
     */
    public static function initialize() {
        self::$_factory = new IocFactory(Skihat::read(self::CONFIG_NAME, array(
            self::DEFAULT_ENGINE => array(
                SKIHAT_IOC_CLASS => 'kernels.caches.engines.file_cache_engine',
                SKIHAT_IOC_PATHS => SKIHAT_PATH_LIBRARY),
            self::RUNTIME_ENGINE => array(
                SKIHAT_IOC_CLASS => 'kernels.caches.engines.file_cache_engine',
                SKIHAT_IOC_PATHS => SKIHAT_PATH_LIBRARY,
                'prefix' => 'runtime')
        )));
    }

    /**
     * 返回所有引擎的名称集。
     *
     * @return array
     */
    public static function engines() {
        return self::$_factory->instances();
    }

    /**
     * 根据$engine参数返回引擎实例。
     *
     * @param string $engine 引擎名称。
     *
     * @return ICacheEngine
     */
    public static function engine($engine) {
        return self::$_factory->instance($engine);
    }

    /**
     * 根据$name，$value，$engine和$expire参数写入缓存信息。
     *
     * @param string $name   缓存名称。
     * @param mixed  $value  缓存值。
     * @param string $engine 缓存引擎，默认为default。
     * @param int    $expire 过期时间，默认1800秒（30分钟）
     *
     * @return mixed
     */
    public static function write($name, $value, $engine = self::DEFAULT_ENGINE, $expire = self::DEFAULT_EXPIRE) {
        $engine = self::engine($engine);
        return $engine->write($name, $value, $expire);
    }

    /**
     * 根据$name和$engine读取缓存值，如果缓存值不存在，或已过期返回false。
     *
     * @param  string $name   缓存名称。
     * @param string  $engine 缓存引擎，默认为default。
     *
     * @return mixed|bool
     */
    public static function read($name, $engine = self::DEFAULT_ENGINE) {
        $engine = self::engine($engine);
        return $engine->read($name);
    }

    /**
     * 根据$name和$engine删除缓存值，并返回删除结果。
     *
     * @param string $name   缓存名称。
     * @param string $engine 缓存引擎，默认为default。
     *
     * @return mixed
     */
    public static function delete($name, $engine = self::DEFAULT_ENGINE) {
        $engine = self::engine($engine);
        return $engine->delete($name);
    }

    /**
     * 根据$engine清除缓存值，并返回处理结果。
     *
     * @param string $engine 缓存引擎，默认为default。
     *
     * @return mixed
     */
    public static function flush($engine = self::DEFAULT_ENGINE) {
        $engine = self::engine($engine);
        return $engine->flush();
    }

    /**
     * @var IocFactory Ioc工厂对象，提供对缓存引擎的管理支持。
     */
    protected static $_factory = null;
}

Cache::initialize();