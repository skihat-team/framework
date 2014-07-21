<?php
/**
 * validators.inc 验证器方法声明文件。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels
 * @version   1.0
 *
 * @created   2012-11-11
 * @modified  2013-09-12
 */

/**
 * 根据$validators和$messages对$array参数进行数据验证，并返回验证错误消息。
 *
 * <h3>validators规则：</h3>
 * <ul>
 *      <li><b>方式一：直接指定验证器</b>:
 *          array('name' => 'required','email' => 'email');
 *      </li>
 *      <li><b>方式二：使用数组参数，指定多个验证器</b>:
 *          array('name' => array('required' => true),'email' = array('email' => true));
 *      </li>
 *      <li><b>方式三：使用数组参数，指定验证器参数</b>:
 *          array('name' => array('strlen' => array(10,20),'required' => true));
 *      <li>
 * </ul>
 *
 * <h3>messages消息规则</h3>
 * <ul>
 *      <li><b>方式一:直接指定字段的错误消息</b>
 *          array('name' => '名称不能为空，并且必须大于10');
 *      </li>
 *      <li><b>方式二：为每种验证方式分别设置错误消息</b>
 *          array('name' => array('required' => '名称不能为空.','strmin' => '名称长度必须大于10'));
 *      </li>
 * </ul>
 *
 * @注意：
 *    validate_validators在验证错误时，当字段发生验证失败后，将会退出字段验证，因此需要注意验证顺序，减少不必要的验证.
 *
 * @param array|ArrayAccess $input      验证值参数。
 * @param array             $validators 验证器集合参数。
 * @param array             $messages   错误消息集。
 *
 * @return array
 */
function validate_validators($input, array $validators, array $messages = array()) {
    $errorMessages = array();

    foreach ($validators as $field => $fieldValidators) {
        if (is_string($fieldValidators)) {
            $fieldValidators = array($fieldValidators => true);
        }

        foreach ($fieldValidators as $validator => $options) {
            $validateMethod = $validator . '_validator';
            if (!($validateMethod($input, $field, $options))) {
                if (isset($messages[$field]) && is_string($messages[$field])) {
                    $errorMessages[$field] = $messages[$field];
                } else if (isset($messages[$field][$validator])) {
                    $errorMessages[$field] = $messages[$field][$validator];
                } else {
                    $errorMessages[$field] = "{$validator} validate fail.";
                }
                break;
            }
        }
    }

    return $errorMessages;
}

/**
 * 非空值验证器，如果验证字段的值非空，则返回true，否则返回false。
 *
 * @param array|ArrayAccess $input 验证值参数。
 * @param string            $field 验证索引字段。
 *
 * @return bool
 */
function required_validator(&$input, $field) {
    return !empty($input[$field]);
}

/**
 * 数值验证器，如果验证字段的值为字值，则返回true，否则返回false。
 *
 * @param array|ArrayAccess $input 验证值参数。
 * @param string            $field 验证字段。
 *
 * @return bool
 */
function numeric_validator(&$input, $field) {
    if (empty($input[$field])) return true;
    return is_numeric($input[$field]);
}

/**
 * 电话号码验证器，如果验证字段值为电话号码，则返回true，否则返回false。
 *
 * @说明
 *      支持坐机(xxx-xxxxxxxxx/xxxx-xxxxxxx)，手机(xxxxxxxxxxx)和400免费电话(400-xxx-xxxxxx)
 *
 * @param array|ArrayAccess $input 验证值参数。
 * @param string            $field 验证字段。
 *
 * @return bool
 */
function telephone_validator(&$input, $field) {
    if (empty($input[$field])) return true;
    return preg_match('/^(\d{11})|(\d{3,4}-\d{7,8})|(400-(\d{3})-(\d{5}))$/', $input[$field]) != false;
}

/**
 * 邮件验证器，如果验证字段值为邮箱则返回true，否则返回false。
 *
 * @param array|ArrayAccess $input 验证值参数。
 * @param string            $field 验证字段。
 *
 * @return bool
 */
function email_validator(&$input, $field) {
    if (empty($input[$field])) return true;
    return filter_var($input[$field], FILTER_VALIDATE_EMAIL) != false;
}

/**
 * Url 验证器，如果验证字段的值为URL地址，则返回true，否则返回false。
 *
 * @param array|ArrayAccess $input 验证值参数。
 * @param string            $field 验证字段。
 *
 * @return bool
 */
function url_validator(&$input, $field) {
    if (empty($input[$field])) return true;
    return filter_var($input[$field], FILTER_VALIDATE_URL) != false;
}

/**
 * IP 验证器，如果验证字段的值为IP地址，则返回true，否则返回false。
 *
 * @param array|ArrayAccess $input 验证值参数。
 * @param string            $field 验证字段。
 *
 * @return bool
 */
function ip_validator(&$input, $field) {
    if (empty($input[$field])) return true;
    return filter_var($input[$field], FILTER_VALIDATE_IP) != false;
}

/**
 * 字符串最小长度验证器，如果验证字段值长度大于或等于$length，则返回true，否则返回false。
 *
 * @param array|ArrayAccess $input  验证值参数。
 * @param string            $field  验证字段。
 * @param int               $length 最小长度选项，默认为10。
 *
 * @return bool
 */
function strmin_validator(&$input, $field, $length = 10) {
    if (empty($input[$field])) return true;
    return mb_strlen($input[$field]) >= $length;
}

/**
 * 字符串最大长度验证器，如果验证字段长度小于或等于$length，则返回true，否则返回false。
 *
 * @param array|ArrayAccess $input  验证值参数。
 * @param string            $field  验证字段。
 * @param int               $length 最大长度选项，默认为32。
 *
 * @return bool
 */
function strmax_validator(&$input, $field, $length = 32) {
    if (empty($input[$field])) return true;
    return mb_strlen($input[$field]) <= $length;
}


/**
 * 字符串长度范围验证器，如果验证字段长度在$range指定的范围内，则返回true，否则返回false。
 *
 * @param array|ArrayAccess $input 验证值参数。
 * @param string            $field 验证字段。
 * @param array             $range 验证范围。
 *
 * @return bool
 */
function strlen_validator(&$input, $field, $range = array()) {
    if (empty($input[$field])) return true;

    list($min, $max) = empty($range) ? array(10, 32) : $range;
    $len = mb_strlen($input[$field]);

    return $len >= $min && $len <= $max;
}

/**
 * 数值最大验证器，如果验证字段值小于或等于$max，则返回true，否则返回false。
 *
 * @param array|ArrayAccess $input 验证值参数。
 * @param string            $field 验证字段。
 * @param int               $max   验证最大值。
 *
 * @return bool
 */
function nummax_validator(&$input, $field, $max = 10000) {
    if (empty($input[$field])) return true;
    return $input[$field] <= $max;
}

/**
 * 数值最小验证器，如果验证字段值大于或等于$min，则返回true，否则返回true。
 *
 * @param array|ArrayAccess $input 验证值参数。
 * @param string            $field 验证字段。
 * @param int               $min   验证最小值。
 *
 * @return bool
 */
function nummin_validator(&$input, $field, $min = 10) {
    if (empty($input[$field])) return true;
    return $input[$field] >= $min;
}

/**
 * 数值范围验证器，如果验证字段值在$range范围内，则返回true，否则返回false。
 *
 * @param ArrayAccess|array $input 验证值参数。
 * @param string            $field 验证字段。
 * @param array             $range 验证范围。
 *
 * @return bool
 */
function numrang_validator(&$input, $field, $range = array()) {
    if (empty($input[$field])) return true;

    list($min, $max) = empty($range) ? array(10, 10000) : $range;
    return $input[$field] >= $min && $input[$field] <= $max;
}

/**
 * 比较验证器，如果验证字段值与$compare字段值相同，则返回true，否则返回false。
 *
 * @param array|ArrayAccess $input   验证值参数。
 * @param string            $field   验证字段。
 * @param string            $compare 比较字段。
 *
 * @return bool
 */
function compare_validator(&$input, $field, $compare = '') {
    if (empty($input[$field]) || empty($compare)) return true;

    return $input[$field] == $input[$compare];
}

/**
 * 正则验证器，如果验证字段值符合正则$pattern，则返回true，否则返回false。
 *
 * @param array|ArrayAccess $input   验证值参数。
 * @param string            $field   验证字段。
 * @param string            $pattern 正则模式。
 *
 * @return bool
 */
function regex_validator(&$input, $field, $pattern = '*') {
    if (empty($input[$field])) return true;
    return preg_match($pattern, $input[$field]) != false;
}

/**
 * 枚举验证器，如果验证字段值包含在$$options中，则返回true，否则返回false。
 *
 * @param  array|ArrayAccess $input   验证值参数。
 * @param  string            $field   验证字段。
 * @param array              $options 允许值范围。
 *
 * @return bool
 */
function enum_validator(&$input, $field, $options = array()) {
    if (empty($input[$field])) return true;
    return in_array($input[$field], (array)$options);
}