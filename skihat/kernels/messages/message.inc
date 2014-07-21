<?php
/**
 * message.inc 提供外部消息发送接口声明。
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

/**
 * 消息组件异常，当发生消息的过程中发生错误将引发本异常。
 *
 * @package skihat.kernels.messages
 */
class MessageException extends SkihatException {
}

/**
 * 消息引擎接口，提供消息的不同发送方式。
 *
 * @package skihat.kernels.messages
 */
interface IMessageEngine {
    /**
     * 根据$address和$message发送通知消息，并返回执行结果。
     *
     * @param mixed $address 发送地址。
     * @param mixed $message 发送消息。
     *
     * @return mixed
     */
    function send($address, $message);
}

/**
 * 消息服务接口对象，提供消息的发送服务。
 *
 * @package skihat.kernels.messages
 */
abstract class Message {
    /**
     * @const string 消息调试名称。
     */
    const DEBUG_NAME = 'kernels/messages';

    /**
     * @const string 消息配置名称。
     */
    const CONFIG_NAME = 'kernels/messages';

    /**
     * @const string 默认引擎名称。
     */
    const DEFAULT_ENGINE = 'default';

    /**
     * 初始化消息组件引擎。
     */
    public static function initialize() {
        self::$_factory = new IocFactory(Skihat::read(self::CONFIG_NAME, array(
            self::DEFAULT_ENGINE => array(
                SKIHAT_IOC_CLASS => 'kernels.messages.engines.email_message_engine',
                SKIHAT_IOC_PATHS => SKIHAT_PATH_LIBRARY)
        )));
    }

    /**
     * 返回全部的消息引擎名称集。
     *
     * @return array
     */
    public static function engines() {
        return self::$_factory->instances();
    }

    /**
     * 根据$engine获取消息通知引擎实例。
     *
     * @param string $name 消息引擎名称。
     *
     * @return IMessageEngine
     */
    public static function engine($name = self::DEFAULT_ENGINE) {
        return self::$_factory->instance($name);
    }

    /**
     * 根据$address，$message和$engine发送通知消息，并返回发送的结果。
     *
     * @param  mixed $address 消息接收地址。
     * @param  mixed $message 通知消息内容。
     * @param string $engine  消息引擎，默认为default。
     *
     * @return mixed
     */
    public static function send($address, $message, $engine = self::DEFAULT_ENGINE) {
        $engine = self::engine($engine);
        return $engine->send($address, $message);
    }

    /**
     * @var IocFactory Ioc工厂，提供消息引擎管理服务。
     */
    protected static $_factory = null;
}
