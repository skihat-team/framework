<?php
/**
 * email_message_engine_test.inc 提供邮件消息通知接口单元测试。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.messages.engines
 * @version   1.0
 *
 * @created   2013-04-05
 * @modified  2013-08-02
 */

# 引用测试启动文件
require_once(dirname(dirname(dirname(dirname(dirname(__FILE__))))) . '/startup.inc');

# 引用测试类
Skihat::import('kernels.messages.message', SKIHAT_PATH_LIBRARY);
Skihat::import('kernels.messages.engines.email_message_engine', SKIHAT_PATH_LIBRARY);

/**
 * 邮件消息通知引擎单元测试类，提供对邮件消息通知的单元测试服务。
 *
 * @package skihat.kernels.messages.engines
 */
class EmailMessageEngineTest extends PHPUnit_Framework_TestCase {

}
