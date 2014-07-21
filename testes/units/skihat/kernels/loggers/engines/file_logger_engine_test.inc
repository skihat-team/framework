<?php
/**
 * file_logger_engine_test.inc 提供文件日志记录方案单元测试声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.loggers
 * @version   1.0
 *
 * @created   2012-07-16
 * @modified  2013-08-05
 */

# 导入测试启动文件。
require_once(dirname(dirname(dirname(dirname(dirname(__FILE__))))) . '/startup.inc');

# 导入文件日志引擎。
Skihat::import('kernels.loggers.logger', SKIHAT_PATH_LIBRARY);
Skihat::import('kernels.loggers.engines.file_logger_engine', SKIHAT_PATH_LIBRARY);

/**
 * 文件日志记录引擎测试类，提供文件日志测试服务支持。
 *
 * @package skihat.kernels.loggers
 */
class FileLoggerEngineTest extends PHPUnit_Framework_TestCase {
    /**
     * 测试记录日志。
     */
    public function testWrite() {
        $engine = Skihat::ioc('#FileLoggerEngine');
        $engine->write('hello', 'hello skihat');

        $this->assertFileExists(SKIHAT_PATH_DATA_LOGGERS . '/run.log');
    }

    /**
     * 测试记录日志，指定文件名。
     */
    public function testWriteForFileName() {
        $engine = Skihat::ioc(array(SKIHAT_IOC_CLASS => '#FileLoggerEngine', 'fileName' => 'more.log'));
        $engine->write('hello', 'hello skihat');

        $this->assertFileExists(SKIHAT_PATH_DATA_LOGGERS . '/more.log');
    }

    /**
     * @see PHPUnit_Framework_TestCase::setUp
     */
    public function setUp() {
        @unlink(SKIHAT_PATH_DATA_LOGGERS . '/run.log');
        @unlink(SKIHAT_PATH_DATA_LOGGERS . '/more.log');
    }
}