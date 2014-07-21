<?php
/**
 * message_test.inc 提供外部消息发送接口声明单元测试类声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.messages
 * @version   1.0
 *
 * @created   2013-04-05
 * @modified  2013-08-02
 */

# 引用测试启动文件。
require_once(dirname(dirname(dirname(dirname(__FILE__)))) . '/startup.inc');

# 引用测试类
Skihat::import('kernels.messages.message', SKIHAT_PATH_LIBRARY);

/**
 * 消息接口单元测试类，提供消息通知接口单元测试服务。
 *
 * @package skihat.kernels.messages
 */
class MessageTest extends PHPUnit_Framework_TestCase {

    /**
     * 测试发送消息。
     */
    public function testSend() {
        Message::send('example@163.com', 'hello skihat');

        $this->assertEquals(1, count(DefaultMessageEngine::$messages));
        $this->assertEquals('example@163.com', DefaultMessageEngine::$messages[0]['address']);
        $this->assertEquals('hello skihat', DefaultMessageEngine::$messages[0]['message']);
    }

    /**
     * 测试发送消息，使用实例引擎。
     */
    public function testSendForExample() {
        Message::send('example@163.com', 'hello skihat', 'example');

        $this->assertEquals(1, count(ExampleMessageEngine::$messages));
        $this->assertEquals('example@163.com', ExampleMessageEngine::$messages[0]['address']);
        $this->assertEquals('hello skihat', ExampleMessageEngine::$messages[0]['message']);
    }

    /**
     * 测试发送消息，使用无效的引擎。
     *
     * @expectedException ConfigException
     */
    public function testSendForInvalidEngine() {
        Message::send('example@163.com', 'hello skihat', 'invalid');
    }

    /**
     * @see PHPUnit_Framework_TestCase::setUp
     */
    public function setUp() {
        Skihat::write(Message::CONFIG_NAME, array(
            Message::DEFAULT_ENGINE => '#DefaultMessageEngine',
            'example' => '#ExampleMessageEngine'
        ));

        Message::initialize();

        DefaultMessageEngine::$messages = array();
        ExampleMessageEngine::$messages = array();
    }
}

/**
 * 默认消息通知接口，为消息通知接口单元测试提供服务支持。
 *
 * @package skihat.kernels.messages
 */
class DefaultMessageEngine implements IMessageEngine {
    public static $messages = array();

    /**
     * @see IMessageEngine::send
     */
    public function send($address, $message) {
        array_push(self::$messages, array('address' => $address, 'message' => $message));
    }

}

/**
 * 实例消息通知接口，为消息通知接口单元测试提供服务支持。
 *
 * @package skihat.kernels.messages
 */
class ExampleMessageEngine implements IMessageEngine {
    public static $messages = array();

    /**
     * @see IMessageEngine::send
     */
    public function send($address, $message) {
        array_push(self::$messages, array('address' => $address, 'message' => $message));
    }

}