<?php
/**
 * logger.inc 提供日志组件接口声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.loggers
 * @version   1.0
 *
 * @created   2012-07-12
 * @modified  2013-08-02
 */

/**
 * 日志异常，当日志组件发生错误时将引发当前异常。
 *
 * @package skihat.kernels.loggers
 */
class LoggerException extends SkihatException {
}

/**
 * 日志引擎类，提供不同日志的记录服务。
 *
 * @package skihat.kernels.loggers
 */
interface  ILoggerEngine {
    /**
     * 根据$subject，$message和$user记录日志信息。
     *
     * @param string $title   日志标题。
     * @param string $message 详细消息内容。
     * @param mixed  $user    当前用户。
     *
     * @return mixed
     */
    function write($title, $message, $user = Logger::DEFAULT_USER);
}

/**
 * 日志记录类
 */
abstract class Logger {
    /**
     * @const string 日志调试名称。
     */
    const  DEBUG_NAME = 'kernels/loggers';

    /**
     * @const string 配置名称。
     */
    const CONFIG_NAME = 'kernels/loggers';

    /**
     * @const string 默认日志引擎。
     */
    const DEFAULT_ENGINE = 'default';

    /**
     * @const string 默认用户。
     */
    const DEFAULT_USER = 'admin';

    /**
     * 初始化日志组件，
     */
    public static function initialize() {
        self::$_factory = new IocFactory(Skihat::read(self::CONFIG_NAME));
    }

    /**
     * 根据$name参数，返回对应的日志引擎。
     *
     * @param string $name 引擎名称。
     *
     * @return ILoggerEngine
     */
    public static function engine($name) {
        return self::$_factory->instance($name);
    }

    /**
     * 返回全部的日志引擎。
     *
     * @return array
     */
    public static function engines() {
        return self::$_factory->instances();
    }

    /**
     * 根据$title,$message,$user和$engine参数记录日志信息。
     *
     * @param string $title   日志标题。
     * @param string $message 日志消息。
     * @param string $user    用户信息，默认为admin。
     * @param string $engine  日志引擎，默认为default。
     */
    public static function write($title, $message, $user = self::DEFAULT_USER, $engine = self::DEFAULT_ENGINE) {
        $engine = self::engine($engine);
        $engine->write($title, $message, $user);
    }

    /**
     * @var IocFactory 日志工厂，提供日志引擎管理服务。
     */
    protected static $_factory = null;
}