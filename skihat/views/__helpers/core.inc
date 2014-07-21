<?php
/**
 * core.inc 提供基本的HTML挂件生成方法。
 *
 * @copyright   2012-2013 Skihat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      Skihat Team
 *
 * @package     skihat.views.__helpers
 * @version     1.0
 *
 * @create      2012-09-16
 * @modify      2013-09-16
 */

/**
 * 将消息转换为闪存消息（一次性消息）。
 *
 * @param IView $view 视图实例。
 */
function flash_message(IView $view) {
    # 从视图中获取消息通知，并将消息写入到SESSION中。
    if ($msg = $view->message()) {
        $_SESSION[array_key_value($msg, 'name', 'flash')] = $msg;
    }
}

/**
 * 根据$view提示构建文本消息字符串，并返回值。
 *
 * @param IView  $view 视图实例。
 * @param string $name 消息名称。
 *
 * @return string
 */
function text_message(IView $view, $name = 'flash') {
    # 获取通知消息。
    if (!($msg = $view->message())) {
        $msg = array_key_pop($_SESSION, $name, false);
    }
    if (!$msg) return '';

    $html = "<div id='message' class='alert alert-{$msg['sign']}'>" .
        "<button type='button' class='close' data-dismiss='alert'>×</button>" . "<h4>{$msg['title']}</h4>";

    if (!empty($msg['message'])) {
        $html .= "<p>{$msg['message']}</p>";
    }

    # 如果指定动作为字符串。
    foreach ($msg['actions'] as $title => $url) {
        $html .= "<a href='$url' class='btn'>$title</a>";
    }

    return $html . '</div>';
}

/**
 * 将textMessage提示消息直接输出。
 *
 * @see textMessage
 */
function _text_message(IView $view, $name = 'flash') {
    echo text_message($view, $name);
}

/**
 * 使用htmlspecialchars方法处理$text参数。
 *
 * @param  string $text 录入文本
 *
 * @return string
 */
function h($text) {
    return htmlspecialchars($text);
}

/**
 * h方法的echo直接输出版本。
 *
 * @see h
 */
function _h($text) {
    echo h($text);
}

/**
 * 直接将文本$text输出到当前环境中。
 *
 * @param mixed $text 文本。
 */
function _e($text) {
    echo $text;
}

/**
 * 使用htmlspecialchars格式化$text文本，并且指定为$len，从$start开始。
 *
 * @param string $text  录入文本
 * @param int    $len   文件长度
 * @param int    $start 开始索引
 *
 * @return string
 */
function hs($text, $len, $start = 0) {
    return mb_substr(htmlspecialchars($text), $start, $len, 'utf-8');
}

/**
 * hs 的直接输出版本。
 *
 * @see hs
 */
function _hs($text, $len, $start = 0) {
    echo hs($text, $len, $start);
}

/**
 * Html element generate 根据$elementName、$options、$addition和$inner生成HTML标签。
 *
 * @param string $elemName 元素名称
 * @param array  $options  元素选项
 * @param string $addition 附加属性
 * @param string $inner    标签内容
 *
 * @return string
 */
function heg($elemName, $options = array(), $addition = '', $inner = '') {
    # 如果$options为数组属性，则生成HTML Attribute
    if (is_array($options)) {
        $options = array_html_attrs($options);
    }

    return "<$elemName $options $addition>$inner</$elemName>";
}

/**
 * 根据$url生成URL地址。
 *
 * @param mixed $url URL生成选项
 *
 * @return string
 */
function url($url = array()) {
    return Router::url($url);
}

/**
 * url的直接输出版本。
 *
 * @see url
 */
function _url($url = array()) {
    echo url($url);
}

/**
 * 根据模块、包、控制器、活动和$id生成URL地址。
 *
 * @param string      $module     模块名称。
 * @param bool|string $package    包名称，默认为SKIHAT_PARAM_DEFAULT_PACKAGE。
 * @param string      $controller 控制器名称，默认为SKIHAT_PARAM_DEFAULT_CONTROLLER。
 * @param string      $action     活动名称，默认为SKIHAT_PARAM_DEFAULT_ACTION。
 * @param string      $id         主键名称，默认为空。
 *
 * @return string
 */
function url_mpca($module, $package = SKIHAT_PARAM_DEFAULT_PACKAGE,
                  $controller = SKIHAT_PARAM_DEFAULT_CONTROLLER,
                  $action = SKIHAT_PARAM_DEFAULT_ACTION, $id = null) {
    return Router::url(array(SKIHAT_PARAM_MODULE => $module,
        SKIHAT_PARAM_PACKAGE => $package,
        SKIHAT_PARAM_CONTROLLER => $controller,
        SKIHAT_PARAM_ACTION => $action,
        SKIHAT_PARAM_ID => $id));
}

/**
 * url_mpca的直接输出版本。
 *
 * @see url_mpca
 */
function _url_mpca($module, $package = SKIHAT_PARAM_DEFAULT_PACKAGE,
                   $controller = SKIHAT_PARAM_DEFAULT_CONTROLLER,
                   $action = SKIHAT_PARAM_DEFAULT_ACTION, $id = null) {
    echo url_mpca($module, $package, $controller, $action, $id);
}

/**
 * 根据$module、$controller、$action和$id生成URL路由地址。
 *
 * @param string $module     模块名称。
 * @param string $controller 控制器名称，默认为SKIHAT_PARAM_DEFAULT_CONTROLLER。
 * @param string $action     活动名称，默认为SKIHAT_PARAM_DEFAULT_ACTION。
 * @param mixed  $id         主键，默认为null。
 *
 * @return string
 */
function url_mca($module, $controller = SKIHAT_PARAM_DEFAULT_CONTROLLER,
                 $action = SKIHAT_PARAM_DEFAULT_ACTION, $id = null) {
    return Router::url(array(
        SKIHAT_PARAM_MODULE => $module,
        SKIHAT_PARAM_PACKAGE => SKIHAT_PARAM_DEFAULT_PACKAGE,
        SKIHAT_PARAM_CONTROLLER => $controller,
        SKIHAT_PARAM_ACTION => $action,
        SKIHAT_PARAM_ID => $id));
}

/**
 * url_mca 函数的直接生成URL地址。
 *
 * @see url_mca
 */
function _url_mca($module, $controller = SKIHAT_PARAM_DEFAULT_CONTROLLER,
                  $action = SKIHAT_PARAM_DEFAULT_ACTION, $id = null) {
    echo url_mca($module, $controller, $action, $id);
}

/**
 * 根据$controller、$action和$id生成URL地址。
 * 注意：SKIHAT_PARAM_MODULE和SKIHAT_PARAM_PACKAGE将会采用当前路由的默认值。
 *
 * @param string $controller 控制器名称，
 * @param string $action     活动名称，默认为SKIHAT_PARAM_DEFAULT_CONTROLLER。
 * @param mixed  $id         主键值，默认为null。
 *
 * @return string
 */
function url_ca($controller, $action = SKIHAT_PARAM_DEFAULT_ACTION, $id = null) {
    return Router::url(array(SKIHAT_PARAM_CONTROLLER => $controller,
        SKIHAT_PARAM_ACTION => $action, SKIHAT_PARAM_ID => $id));
}

/**
 * url_ca 的直接输出版本。
 *
 * @see url_ca
 */
function _url_ca($controller = SKIHAT_PARAM_DEFAULT_CONTROLLER,
                 $action = SKIHAT_PARAM_DEFAULT_ACTION, $id = null) {
    echo url_ca($controller, $action, $id);
}

/**
 * 根据$action和$id生成URL地址。
 * 注意：SKIHAT_PARAM_MODULE、SKIHAT_PARAM_PACKAGE、SKIHAT_PARAM_CONTROLLER 将会采用当前路由的默认值。
 *
 * @param string $action 活动名称。
 * @param int    $id     主键值。
 *
 * @return string
 */
function url_a($action, $id = null) {
    return Router::url(array(SKIHAT_PARAM_ACTION => $action, SKIHAT_PARAM_ID => $id));
}

/**
 * url_a 的直接输出版本。
 *
 * @see url_a
 */
function _url_a($action, $id = null) {
    echo url_a($action, $id);
}

/**
 * 根据$controller、$action、$param和$value生成URL路径。
 *
 * @param string $controller 控制器名称。
 * @param string $action     活动名称。
 * @param string $param      参数名称。
 * @param  mixed $value      参数值。
 *
 * @return string
 */
function url_capv($controller, $action, $param, $value) {
    return Router::url(array($param => $value,
        SKIHAT_PARAM_CONTROLLER => $controller,
        SKIHAT_PARAM_ACTION => $action));
}

/**
 * url_capv的直接输出版本。
 *
 * @see url_capv
 */
function _url_capv($controller, $action, $param, $value) {
    echo url_capv($controller, $action, $param, $value);
}

/**
 * 根据$action、$param和$value生成url路径。
 *
 * @param string $action 活动名称
 * @param string $param  参数名称。
 * @param mixed  $value  参数值。
 *
 * @return string
 */
function url_apv($action, $param, $value) {
    return Router::url(array(SKIHAT_PARAM_ACTION => $action, $param => $value));
}

/**
 * url_apv的直接输出版本。
 */
function _url_apv($action, $param, $value) {
    echo url_apv($action, $param, $value);
}

/**
 * 根据$action和$id生成完整URL地址参数。
 *
 * @param string $action 活动名称。
 * @param null   $id     编号值。
 *
 * @return string
 */
function url_af($action, $id = null) {
    return Router::url(array(SKIHAT_PARAM_ACTION => $action, SKIHAT_PARAM_ID => $id, Router::FULL_URL => true));
}

/**
 * @see url_af
 */
function _url_af($action, $id = null) {
    echo url_af($action, $id);
}

/**
 * 根据$text、$url和$options生成A标签HTML。
 *
 * @param string $text    超连接文本。
 * @param mixed  $url     超连接地址。
 * @param mixed  $options 选项。
 *
 * @return string
 */
function link_to($text, $url = array(), $options = array()) {
    # 如果$url为数组，使用Router生成URL地址信息。
    if (is_array($url)) $url = Router::url($url);

    # 使用_heg生成HTML标签。
    return heg('a', $options, "href=\"$url\"", $text);
}

/**
 * link_to的直接输出版本。
 *
 * @see link_to
 */
function _link_to($text, $url = array(), $options = array()) {
    echo link_to($text, $url, $options);
}

/**
 * 使用$image、$url和$options生成带图片的A标签HTML。
 *
 * @param array $image   图片选项
 * @param array $url     URL地址选项
 * @param array $options 选项
 *
 * @return string
 */
function link_image($image, $url = array(), $options = array()) {
    return link_to(heg('img', $image), $url, $options);
}

/**
 * link_image的直接输出版本。
 *
 * @see link_image
 */
function _link_image($image, $url = array(), $options = array()) {
    echo link_image($image, $url, $options);
}

/**
 * 使用$email生成带mailto的A标签HTML。
 *
 * @param string $email   邮件地址
 * @param array  $options 选项
 *
 * @return string
 */
function mail_to($email, $options = array()) {
    $options['href'] = "mailto:$email";
    # 使用heg生成A标签
    return heg('a', $options, '', $email);
}

/**
 * mail_to的直接输出版本。
 *
 * @see mail_to
 */
function _mail_to($email, $options = array()) {
    echo mail_to($email, $options);
}

/**
 * 根据$name和格式参数输出i18n国际化内容。
 *
 * @see SKihat::i18n
 *
 * @param string $name 标志名称
 *
 * @return string
 */
function i18n($name) {
    $args = func_get_args();
    $args[0] = Skihat::i18n($name);

    return call_user_func_array('sprintf', $args);
}

/**
 * i18n 的直接输出版本。
 *
 * @see i18n
 */
function _i18n($name) {
    echo call_user_func_array('i18n', func_get_args());
}

/**
 * 根据$date返回格式化日期字符串。
 *
 * @param integer $date   时间戳
 * @param string  $format 默认日期格式化
 *
 * @return string
 */
function nice_date($date, $format = SKIHAT_I18N_DATE) {
    return date($format, $date);
}

/**
 * nice_date 的直接输出版本。
 *
 * @see nice_date
 */
function _nice_date($date, $format = SKIHAT_I18N_DATE) {
    echo nice_date($date, $format);
}

/**
 * 根据$date返回格式化时间字符串。
 *
 * @param integer $date   时间戳
 * @param string  $format 格式器
 *
 * @return string
 */
function nice_date_time($date, $format = SKIHAT_I18N_DATE_TIME) {
    return date($format, $date);
}

/**
 * nice_datetime的直接输出版本。
 *
 * @see nice_date_time
 */
function _nice_date_time($date, $format = SKIHAT_I18N_DATE_TIME) {
    echo nice_date_time($date, $format);
}

/**
 * 根据$time返回格式化时间字符串。
 *
 * @param integer $time   时间戳
 * @param string  $format 格式
 *
 * @return string
 */
function nice_time($time, $format = SKIHAT_I18N_TIME) {
    return date($format, $time);
}

/**
 * nice_time 的直接输出版本。
 *
 * @see nice_time
 */
function _nice_time($time, $format = SKIHAT_I18N_TIME) {
    echo nice_time($time, $format);
}

/**
 * 根据$date返回时间的周信息。
 *
 * @param integer $date   时间
 * @param string  $format 格式
 *
 * @return string
 */
function nice_week($date, $format = SKIHAT_I18N_WEEK) {
    return date($format, $date);
}

/**
 * nice_week 的直接输出版本。
 *
 * @see nice_week
 */
function _nice_week($date, $format = SKIHAT_I18N_WEEK) {
    echo nice_week($date, $format);
}

/**
 * 根据$date返回过去的时间字符串的友好形式。
 *
 * @param int $date 时间戳
 *
 * @return string
 */
function nice_last_time($date) {
    # 获取时间差
    $time = time() - $date;

    if ($time < 60) return $time . '秒前';
    if ($time < 3600) return floor($time / 60) . '分前';
    if ($time < 86400) return floor($time / 3600) . '小时前';
    if ($time < 4320000) return floor($time / 86400) . '天前';
    else return '3个月前';
}

/**
 * nice_last_time的直接输出版本。
 *
 * @see nice_last_time
 */
function _nice_last_time($date) {
    echo nice_last_time($date);
}

/**
 * 根据$decimal和$format生成友好货币字符串。
 *
 * @param int    $decimal
 * @param string $format
 *
 * @return string
 */
function nice_currency($decimal, $format = SKIHAT_I18N_CURRENCY) {
    return sprintf($format, $decimal);
}

/**
 * nice_currency 的直接输出版本。
 *
 * @see nice_currency
 */
function _nice_currency($decimal, $format = SKIHAT_I18N_CURRENCY) {
    echo nice_currency($decimal, $format);
}
