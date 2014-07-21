<?php
/**
 * core.inc 提供框架核心方法声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels
 * @version   1.0
 *
 * @created   2012-08-07
 * @modified  2013-08-12
 */

/**
 * Camel大写命名规范，每一个单词首字母小写，其它单词首字母大写。
 *
 * @example
 *  <pre>
 *      camel_upper('hello_world'); # helloWorld
 *      camel_upper('HelloWorld');  # helloWorld
 *      camel_upper('_hello_world'); # helloWorld
 *  </pre>
 *
 * @param string $word 单词。
 *
 * @return string
 */
function camel_upper($word) {
    $word = str_replace(
        array('_a', '_b', '_c', '_d', '_e', '_f', '_g', '_h', '_i', '_j', '_k', '_l', '_m', '_n', '_o', '_p', '_q', '_r', '_s', '_t', '_u', '_v', '_w', '_x', '_y', '_z'),
        array('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'),
        $word);

    $word[0] = strtolower($word[0]);
    return $word;
}

/**
 * Camel小写命名规范，所有单词小写，单词之间"_"分隔。
 *
 * @example
 *  <pre>
 *      camel_lower('helloWorld');   # hello_world
 *      camel_lower('HelloWorld');   # hello_world
 *  </pre>
 *
 * @param string $word 单词。
 *
 * @return string
 */
function camel_lower($word) {
    return trim(str_replace(
        array('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'),
        array('_a', '_b', '_c', '_d', '_e', '_f', '_g', '_h', '_i', '_j', '_k', '_l', '_m', '_n', '_o', '_p', '_q', '_r', '_s', '_t', '_u', '_v', '_w', '_x', '_y', '_z'),
        $word), '_');

}

/**
 * Pascal 命名规范，所有单词首字母大写。
 *
 * @example
 *  <pre>
 *      pascal('helloWorld');  # HelloWorld
 *      pascal('hello_world'); # HelloWorld
 *  </pre>
 *
 * @param string $word 单词
 *
 * @return string
 */
function pascal($word) {
    return ucfirst(str_replace(
        array('_a', '_b', '_c', '_d', '_e', '_f', '_g', '_h', '_i', '_j', '_k', '_l', '_m', '_n', '_o', '_p', '_q', '_r', '_s', '_t', '_u', '_v', '_w', '_x', '_y', '_z'),
        array('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'),
        $word));
}

/**
 * 根据数组$array索引$key返回值，如果指定$key的索引不存在，则返回$default值。
 *
 * @param array $array   数组参数。
 * @param mixed $key     索引。
 * @param mixed $default 默认值。
 *
 * @return mixed
 */
function array_key_value(array &$array, $key, $default = false) {
    return isset($array[$key]) ? $array[$key] : $default;
}

/**
 * 根据$array索引$key删除值，并返回删除$key的值，如果$key不存在则返回$default参数值。
 *
 * @param array $array   数组参数。
 * @param mixed $key     索引。
 * @param mixed $default 默认值。
 *
 * @return mixed
 */
function array_key_pop(array &$array, $key, $default = false) {
    if (isset($array[$key])) {
        $default = $array[$key];
        unset($array[$key]);
    }

    return $default;
}

/**
 * 根据$array和$callback执行数组过滤，并返回过滤的结果。
 *
 * @注意：
 *      回调方法签名：function callback($key);
 *
 * @param array    $array    数组参数。
 * @param Callback $callback 回调方法。
 *
 * @return array
 */
function array_key_filter(array &$array, $callback) {
    $rows = array();

    foreach ($array as $key => $val) {
        if (call_user_func($callback, $key)) {
            $rows[$key] = $val;
        }
    }

    return $rows;
}

/**$
 * 将join数组的值，追加到$input后面。
 *
 * @param array $input 被追加数组。
 * @param array $join  追加数组。
 *
 * @return array
 */
function array_append(array &$input, array $join) {
    # 迭代值，并追加到input之后。
    foreach ($join as $value) {
        array_push($input, $value);
    }

    return $input;
}

/**
 * 为指定数组的值附加字符串，如果指定$key不存在则设置附加字符串为数组$key的值。
 *
 * @example
 *  <pre>
 *      $arr = array('name' => 'chenJ);
 *      array_join_str($arr,'name',' programmer'); # $arr值为array('name' => 'chenJ programmer)
 *      array_join_str($arr,'age','18');  # $arr 值为array('name' => 'chenJ programmer','age' => '18');
 *  </pre>
 *
 * @param Array  $arr  录入数组
 * @param mixed  $key  关键字
 * @param string $join 附加数组值
 *
 * @return void
 */
function array_join_str(array &$arr, $key, $join) {
    # 如果附加值为false返回；
    if (empty($join)) return;

    # 如果数组右眼包含指定KEY的值；
    if (isset($arr[$key])) {
        $arr[$key] = $arr[$key] . $join;
    } else {
        $arr[$key] = $join;
    }
}

/**
 * 将$arr数组生成为html属性标签，KEY将生成为属性名称、VALUE生成为属性值。
 *
 * <b>注意:</b>属性值将会自动调用htmlspecialchars进行过滤处理。
 *
 * @example
 *  <pre>
 *      # name="emp_id" class="header"
 *      array_html_attrs(array('name' => 'emp_id','class' => 'header'));
 *  </pre>
 *
 * @param Array $arr 属性值
 *
 * @return string
 */
function array_html_attrs(array $arr) {
    # 如果指定属性不是数组参数，直接返回；
    if (!is_array($arr)) return $arr;

    # 生成完整参数；
    $html = '';
    foreach ($arr as $name => $value) {
        $value = htmlspecialchars($value);
        $html = "$html $name=\"$value\"";
    }

    return $html;
}

/**
 * 返回客户请求方法名称。
 */
function client_method() {
    $method = strtolower($_SERVER['REQUEST_METHOD']);

    # 如果请求方法不是GET，则判断是否包含方法参数。
    if ($method != 'get' && isset($_POST[SKIHAT_PARAM_METHOD])) {
        return strtolower($_POST[SKIHAT_PARAM_METHOD]);
    }

    return $method;
}

/**
 * 返回客户端请求的URL地址。
 *
 * @return string
 */
function client_url() {
    return $_SERVER['REQUEST_URI'];
}

/**
 * 返回客户端请求的引用地址。
 */
function client_refer() {
    return $_SERVER['HTTP_REFERER'];
}

/**
 * 返回客户端请求的脚本名称。
 *
 * @return string
 */
function client_script_name() {
    return substr($_SERVER['SCRIPT_NAME'], 1);
}

/**
 * 返回客户请求的IP地址。
 *
 * @return string
 */
function client_address() {
    if (getenv("HTTP_CLIENT_IP")) return getenv("HTTP_CLIENT_IP");
    if (getenv("HTTP_X_FORWARDED_FOR")) return getenv("HTTP_X_FORWARDED_FOR");
    if (getenv("REMOTE_ADDR")) return getenv("REMOTE_ADDR");
    return "Unknow";
}

/**
 * 返回客户端浏览器信息。
 *
 * @return string
 */
function d() {
    return $_SERVER['HTTP_USER_AGENT'];
}

/**
 * 返回客户端请求的语言信息。
 *
 * @return array
 */
function client_languages() {
    # zh-CN,zh;q=0.8,xh;q=0.6
    $lang = explode(';', $_SERVER['HTTP_ACCEPT_LANGUAGE']);
    return explode(',', $lang[0]);
}


/**
 * 返回一个布尔值，表示当前环境是否允许使用缓存。
 *
 * @return bool
 */
function enable_cached() {
    return SKIHAT_DEBUG <= SKIHAT_DEBUG_CACHE;
}

/**
 * 根据调用参数生成完整路径。
 *
 * @return string
 */
function full_path() {
    return implode(DIRECTORY_SEPARATOR, func_get_args());
}

/**
 * 根据调用参数，生成完整的文件名称。
 *
 * @return string
 */
function full_file() {
    return implode(SKIHAT_PACKAGE_SEPARATOR, func_get_args());
}

/**
 * 非空值判断，如果$val为非空值返回true，否则返回false。
 *
 * @param mixed $val 值。
 *
 * @return bool
 */
function not_null($val) {
    return !is_null($val);
}

/**
 * 非空值判断，如果$val为非empty值返回true，否则返回false。
 *
 * @param mixed $val 值
 *
 * @return bool
 */
function not_empty($val) {
    return !empty($val);
}

/**
 * 非数值判断，如果$val值不是数值变量返回true，否则返回false。
 *
 * @param mixed $val 值
 *
 * @return bool
 */
function not_numeric($val) {
    return !is_numeric($val);
}
