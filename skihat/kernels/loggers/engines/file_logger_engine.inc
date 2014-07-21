<?php
/**
 * file_logger_engine.inc 提供文件日志记录引擎声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.loggers
 * @version   1.0
 *
 * @created   2012-07-12
 * @modified  2013-08-06
 */

/**
 * 文件日志引擎，提供将日志信息保存到文件中的服务。
 *
 * @package skihat.kernels.loggers
 */
class FileLoggerEngine implements ILoggerEngine {

    /**
     * @const string 默认记录模板。
     */
    const DEFAULT_TEMPLATE = "---------------------------------\r\n title:{:title}\r\n message:{:message}\r\n user:{:user}\r\n datetime:{:datetime}";

    /**
     * @const string 默认文件名称。
     */
    const DEFAULT_FILE = 'run.log';

    /**
     * @var string 记录模板，允许设置最后的记录方式。
     */
    public $template = self::DEFAULT_TEMPLATE;

    /**
     * @var string 日志文件名称，默认为run.log。
     */
    public $fileName = self::DEFAULT_FILE;

    /**
     * @see LoggerEngine::write
     */
    public function write($title, $message, $user = Logger::DEFAULT_USER) {
        $this->_writeToFile($this->_templateMessage($title, $message, $user));
    }

    /**
     * 将$message记录到日志文件中。
     *
     * @param string $message 完整的消息信息。
     */
    protected function _writeToFile($message) {
        file_put_contents($this->_fullFileName(), $message, FILE_APPEND);
    }

    /**
     * 返回日志文件的完整文件名。
     *
     * @return string
     */
    protected function _fullFileName() {
        return SKIHAT_PATH_DATA_LOGGERS . DIRECTORY_SEPARATOR . $this->fileName;
    }

    /**
     * 根据$subject，$message和$user参数生成并返回完整的消息内容。
     *
     * @param string $title   日志标题。
     * @param string $message 完整内容。
     * @param string $user    当前用户。
     *
     * @return string
     */
    protected function _templateMessage($title, $message, $user) {
        return str_replace(
            array('{:user}', '{:message}', '{:title}', '{:datetime}'),
            array($user, $message, $title, date('Y-m-d h:i:s')), $this->template);
    }
}