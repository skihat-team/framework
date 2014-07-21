<?php
/**
 * logger_test.inc 提供日志组件接口单元测试声明。
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

# 启动测试服务。
require_once(dirname(dirname(dirname(dirname(__FILE__)))) . '/startup.inc');

# 导入被测试文件。
Skihat::import('kernels.loggers.logger', SKIHAT_PATH_LIBRARY);

/**
 * 日志接口单元测试类，提供日志接口的单元测试服务。
 *
 * @package skihat.kernels.loggers
 */
class LoggerTest extends PHPUnit_Framework_TestCase {

    /**
     * 测试写入默认值。
     */
    public function testWriteForDefault() {
        Logger::write('hello', 'hello skihat');

        $this->assertEquals('hello', DefaultLoggerEngine::$logs[0]['title']);
        $this->assertEquals('hello skihat', DefaultLoggerEngine::$logs[0]['message']);
        $this->assertEquals('admin', DefaultLoggerEngine::$logs[0]['user']);
    }

    /**
     * 测试写入，使用示例。
     */
    public function testWriteForExample() {
        Logger::write('hello', 'hello skihat', 'user', 'example');

        $this->assertEquals('hello', ExampleLoggerEngine::$logs[0]['title']);
        $this->assertEquals('hello skihat', ExampleLoggerEngine::$logs[0]['message']);
        $this->assertEquals('user', ExampleLoggerEngine::$logs[0]['user']);
    }

    /**
     * 测试写入，使用无效的引擎。
     *
     * @expectedException ConfigException
     */
    public function testWriteForInvalidEngine() {
        Logger::write('hello', 'hello skihat', 'user', 'invoke');
    }

    /**
     * @see PHPUnit_Framework_Test::setUp
     */
    public function setUp() {
        Skihat::write(Logger::CONFIG_NAME, array(
            Logger::DEFAULT_ENGINE => '#DefaultLoggerEngine',
            'example' => '#ExampleLoggerEngine'
        ));

        DefaultLoggerEngine::$logs = array();
        ExampleLoggerEngine::$logs = array();

        Logger::initialize();
    }
}


/**
 * 默认日志引擎，提供日志接口测试所需的服务支持。
 *
 * @package skihat.kernels.loggers
 */
class DefaultLoggerEngine implements ILoggerEngine {
    public static $logs = array();

    public function write($title, $message, $user = Logger::DEFAULT_USER) {
        array_push(self::$logs, array('title' => $title, 'message' => $message, 'user' => $user));
    }
}

/**
 * 实例日志引擎，提供日志接口测试所需的服务支持。
 *
 * @package skihat.kernels.loggers
 */
class ExampleLoggerEngine implements ILoggerEngine {
    public static $logs = array();

    public function write($title, $message, $user = Logger::DEFAULT_USER) {
        array_push(self::$logs, array('title' => $title, 'message' => $message, 'user' => $user));
    }
}