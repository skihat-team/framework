<?php

/**
 * cache_filter.inc 提供活动缓存过滤器声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.controllers.filters
 * @version   1.0
 *
 * @created   2013-04-08
 * @modified  2013-09-18
 */

/**
 * 缓存过滤器，提供控制器活动的缓存支持。
 *
 * @package skihat.controllers.filters
 */
class CacheFilter implements IActionFilter {
    /**
     * @var array 允许进行缓存的活动方法名称。
     */
    public $actionMethods = array();

    /**
     * @var string 使用的缓存引擎。
     */
    public $engine = 'default';

    /**
     * @var int 缓存有效时间（秒）。
     */
    public $expire = 3600;

    /**
     * @see IActionFilter::initialize
     */
    public function initialize(Controller $controller) {
        $this->_controller = $controller;
    }

    /**
     * @see IActionFilter::invoke
     */
    public function invoke($actionMethod) {
        # 如果不允许缓存，直接返回。
        if (!$this->_enableCached($actionMethod)) return true;

        # 如果读取缓存成功，则将缓存值写入到响应实例中。
        if ($cache = Cache::read($this->_cacheName(), $this->engine)) {
            $this->_controller->response->copy($cache);
            return false;
        }

        $this->_enableCache = true;
        return true;
    }

    /**
     * @see IActionFilter::render
     */
    public function render(array &$options) {
        return true;
    }

    /**
     * @see IActionFilter::complete
     */
    public function complete() {
        if ($this->_enableCache) {
            Cache::write(
                $this->_cacheName(),
                $this->_controller->response,
                $this->engine, $this->expire
            );
        }
    }

    /**
     * 根据$actionMethod返回一个布尔值，表示是否允许进行缓存。
     *
     * @param string $actionMethod 活动名称。
     *
     * @return bool
     */
    protected function _enableCached($actionMethod) {
        # 如果当前不开启缓存或请求方法不为GET请求。
        if (!enable_cached() || client_method() != 'get') return false;
        if ($this->actionMethods == '*') return true;

        # 返回判断当前请求为允许活动方法。
        return in_array($actionMethod, (array)$this->actionMethods);
    }

    /**
     * 返回当前请求的缓存名称。
     *
     * @return string
     */
    function _cacheName() {
        $all = $this->_controller->request->queryAll();
        return 'cache-filter-' . implode('-', array_keys($all)) . '-' . implode('-', $all);
    }

    /**
     * @var Controller 控制器。
     */
    protected $_controller = null;

    /**
     * @var bool 是否开启缓存。
     */
    protected $_enableCache = false;

}

