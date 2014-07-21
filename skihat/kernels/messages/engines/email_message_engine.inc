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

/**
 * 邮件消息引擎，提供通知邮件进行通知的服务支持。
 *
 * @package skihat.kernels.messages.engines
 */
class EmailMessageEngine implements IMessageEngine {
    /**
     * @const string 地址常量，接收人。
     */
    const ADDRESS_TO = 'to';

    /**
     * @const string 地址常量，抄送。
     */
    const ADDRESS_CC = 'cc';

    /**
     * @const string 消息常量，使用消息的模板。
     */
    const MESSAGE_TEMPLATE = 'template';

    /**
     * @const string 消息常量，标题。
     */
    const MESSAGE_subject = 'subject';

    /**
     * @const string 消息常量，内容。
     */
    const MESSAGE_BODY = 'body';

    /**
     * @var string 默认邮件主题。
     */
    public $defaultSubject = 'Email message notify';

    /**
     * @var bool 使用布尔值，表示是否一次性发送，一次性发送会将所有接收者一起发送。
     */
    public $sendAll = true;

    /**
     * @see IMessageEngine::send
     */
    public function send($address, $message) {

    }

    /**
     * 根据$mailTo，$subject,$message和$cc发送邮件信息。
     *
     * @param string      $to      邮件接收人。
     * @param string      $subject 邮件主题。
     * @param string      $message 邮件消息。
     * @param bool|string $cc      抄送。
     *
     * @return         mixed
     */
    protected function _sendMail($to, $subject, $message, $cc = false) {
        return mail($to, $subject, $message);
    }
}