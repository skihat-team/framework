<?php
/**
 * view.inc 提供视图组件的相关类和接口声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.views
 * @version   1.0
 *
 * @created   2012-07-23
 * @modified  2013-08-13
 */

/**
 * 视图组件异常类，当视图组件发生错误时引发本异常。
 *
 * @package skihat.views
 */
class ViewException extends SkihatException {
}

/**
 * 视图接口，提供通过模板进行渲染的能力。
 *
 * @package skihat.views
 */
interface IView {
    /**
     * @const string 模板扩展名。
     */
    const TEMPLATE_EXT = '.stp';

    /**
     * @const string  视图选项，模板。
     */
    const OPT_TEMPLATE = 'template';

    /**
     * @const string 指定视图模板的基本路径。
     */
    const OPT_BASE_PATHS = 'paths';

    /**
     * @const string 视图选项，pass参数值。
     */
    const OPT_PASS = 'pass';

    /**
     * @const string 视图选项，缓存选项。
     */
    const OPT_CACHE = 'cache';

    /**
     * @const string 缓存引擎。
     */
    const OPT_CACHE_ENGINE = 'cacheEngine';

    /**
     * @const string 视图选项，缓存有效期（秒）。
     */
    const OPT_CACHE_EXPIRE = 'cacheExpire';

    /**
     * 使用$controller和$options初始化IView视图接口实例。
     *
     * @param Controller $controller 控制器对象。
     * @param array      $options    渲染选项。
     */
    function initialize(Controller $controller, array $options);

    /**
     * 根据$vars设置视图模板的相关变量。
     *
     * @param array $vars
     *
     * @return mixed
     */
    function assignAll(array $vars);

    /**
     * 获取或设置视图的通知消息，如果$message为空值表示仅仅获取消息。
     *
     * @param mixed $message 通知消息。
     *
     * @return mixed
     */
    function message($message = NULL);

    /**
     * 将模板渲染结果填充到控制器的应用响应实例中。
     */
    function inflate();
}