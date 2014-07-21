<?php
/**
 * form.inc 提供表单生成服务。
 *
 * @copyright   2012-2013 Skihat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      Skihat Team
 *
 * @package     skihat.views._helpers
 * @version     1.0
 *
 * @create      2013-10-16
 * @modify      2013-10-16
 */

/**
 * 表单生成器挂件，提供生成表单的服务。
 *
 * @package skihat.views._helpers
 */
class FormBuilder {
    /**
     * @const string 字段分隔符。
     */
    const FIELD_SEPARATOR = '.';

    /**
     * @const string 标题选项。
     */
    const OPT_TITLE = 'title';

    /**
     * @const string 选项格式器。
     */
    const OPT_FORMATTER = 'formatter';

    /**
     * @var string 错误类名称。
     */
    public $error = ' input-error';

    /**
     * @var IView 表单相关视图。
     */
    protected $_context = null;

    /**
     * 使用$context初始化表单生成器。
     *
     * @param IView $context 表单生成器上下文件视图对象。
     */
    public function __construct(IView $context) {
        $this->_context = $context;
    }

    /**
     * 根据$attrs生成表单开始标志,$attrs会自动生成为表单属性。
     *
     * @param array $attrs 开始选项。
     *
     * @return string
     */
    public function begin(array $attrs = array()) {
        # 根据$attrs生成生成默认选项。
        $attrs = $this->_defaultFormAttributes($attrs);
        $method = array_key_pop($attrs, SKIHAT_PARAM_METHOD, false);
        $html = '<form' . array_html_attrs($attrs) . '>';

        if ($method) {
            $html .= '<div style="display:none;">';
            $html .= sprintf('<input type="hidden" name="%s" value="%s" />', SKIHAT_PARAM_METHOD, $method);
            $html .= '</div>';
        }

        return $html;
    }

    /**
     * 返回表单结束标志。
     */
    public function end() {
        return '</form>';
    }

    /**
     * 根据$field和$attrs生成表单label标签。
     *
     * @param string $field 视图字段。
     * @param array  $attrs 选项。
     *
     * @return string
     */
    public function label($field, $attrs = array()) {
        list($var, $prop) = $this->_splitField($field);

        if (!is_array($attrs)) {
            $title = $attrs;
            $attrs = array();
        } else if (!isset($attrs[self::OPT_TITLE])) {
            $title = $this->_defaultFieldTitle($var, $prop);
        } else {
            $title = array_key_pop($attrs, self::OPT_TITLE);
        }

        # 生成默认属性值。
        if (!isset($attrs['id'])) $attrs['id'] = $this->_defaultFieldID($var, $prop, 'label');
        if (!isset($attrs['for'])) $attrs['for'] = $this->_defaultFieldID($var, $prop);

        return '<label' . array_html_attrs($attrs) . '>' . $title . '</label>';
    }

    /**
     * 根据$field和$attrs生成隐藏域录入框。
     *
     * @param string $field 字段。
     * @param mixed  $attrs 选项，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function hidden($field, $attrs = array()) {
        return $this->_inputTag($field, 'hidden', $attrs);
    }

    /**
     * 根据$field和$attrs生成文本录入框。
     *
     * @param  string $field 字段。
     * @param mixed   $attrs 选项，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function text($field, $attrs = array()) {
        return $this->_inputTag($field, 'text', $attrs);
    }

    /**
     * 根据$field和$attrs生成email录入框 。
     *
     * @param    string $field 字段名称。
     * @param mixed     $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function email($field, $attrs = array()) {
        return $this->_inputTag($field, 'email', $attrs);
    }

    /**
     * 根据$field和$attrs生成password录入框 。
     *
     * @param    string $field 字段名称。
     * @param mixed     $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function password($field, $attrs = array()) {
        return $this->_inputTag($field, 'password', $attrs);
    }

    /**
     * 根据$field和$attrs生成文件录入框 。
     *
     * @param    string $field 字段名称。
     * @param mixed     $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function file($field, $attrs = array()) {
        return $this->_inputTag($field, 'file', $attrs);
    }

    /**
     * 根据$field和$attrs生成文件录入框 。
     *
     * @param string $field 字段名称。
     * @param mixed  $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function url($field, $attrs = array()) {
        return $this->_inputTag($field, 'url', $attrs);
    }

    /**
     * 根据$field和$attrs生成number录入框 。
     *
     * @param string $field 字段名称。
     * @param mixed  $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function number($field, $attrs = array()) {
        return $this->_inputTag($field, 'number', $attrs);
    }

    /**
     * 根据$field和$attrs生成range录入框 。
     *
     * @param string $field 字段名称。
     * @param mixed  $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function range($field, $attrs = array()) {
        return $this->_inputTag($field, 'range', $attrs);
    }

    /**
     * 根据$field和$attrs生成datetime录入框 。
     *
     * @param string $field 字段名称。
     * @param mixed  $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function datetime($field, $attrs = array()) {
        return $this->_inputTag($field, 'datetime', $attrs);
    }

    /**
     * 根据$field和$attrs生成date录入框 。
     *
     * @param string $field 字段名称。
     * @param mixed  $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function date($field, $attrs = array()) {
        return $this->_inputTag($field, 'date', $attrs);
    }

    /**
     * 根据$field和$attrs生成time录入框 。
     *
     * @param string $field 字段名称。
     * @param mixed  $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function time($field, $attrs = array()) {
        return $this->_inputTag($field, 'time', $attrs);
    }

    /**
     * 根据$field和$attrs生成month录入框 。
     *
     * @param string $field 字段名称。
     * @param mixed  $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function month($field, $attrs = array()) {
        return $this->_inputTag($field, 'month', $attrs);
    }

    /**
     * 根据$field和$attrs生成week录入框 。
     *
     * @param string $field 字段名称。
     * @param mixed  $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function week($field, $attrs = array()) {
        return $this->_inputTag($field, 'week', $attrs);
    }

    /**
     * 根据$field和$attrs生成textarea录入框 。
     *
     * @param string $field 字段名称。
     * @param mixed  $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function textarea($field, $attrs = array()) {
        list($var, $prop) = $this->_splitField($field);

        $attrs = $this->_defaultInputAttributes($var, $prop, $attrs);
        if (!isset($attrs['value']) && ($value = $this->_defaultFieldValue($var, $prop)) !== null) {
            $attrs['value'] = $value;
        }
        $inner = array_key_pop($attrs, 'value', '');

        return '<textarea' . array_html_attrs($attrs) . '>' . $inner . '</textarea>';
    }

    /**
     * 根据$field和$attrs生成checkbox录入框 。
     *
     * @param string $field 字段名称。
     * @param mixed  $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function checkbox($field, $attrs = array()) {
        list($var, $prop) = $this->_splitField($field);

        $attrs = $this->_defaultInputAttributes($var, $prop, $attrs);
        $value = array_key_value($attrs, 'value');
        if ($value == $this->_defaultFieldValue($var, $prop)) {
            $attrs['checked'] = '1';
        }

        return $this->_inputTag($field, 'hidden', '0') . '<input type="checkbox"' . array_html_attrs($attrs) . ' />';
    }

    /**
     * 根据$field和$attrs生成radio录入框 。
     *
     * @param string $field 字段名称。
     * @param mixed  $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function radio($field, $attrs = array()) {
        list($var, $prop) = $this->_splitField($field);

        # 生成radio的不同ID值。
        $attrs = $this->_defaultInputAttributes($var, $prop, $attrs);
        $attrs['id'] = $this->_defaultFieldID($var, $prop, array_key_value($attrs, 'value'));

        # 判断是否存在默认值，如果字段的值不为空，则直接设置选中状态。
        if (($value = $this->_defaultFieldValue($var, $prop)) !== null) {
            if ($value == $attrs['value']) {
                $attrs['checked'] = '1';
            }
        }

        return '<input type="radio"' . array_html_attrs($attrs) . ' />';
    }

    /**
     * 根据$field和$attrs生成select录入框 。
     *
     * @param string $field 字段名称。
     * @param mixed  $attrs 选项值，如果为非数组值则表示指定值属性。
     *
     * @return string
     */
    public function select($field, $attrs = array()) {
        list($var, $prop) = $this->_splitField($field);

        # 生成录入框默认选项，并提取特殊值。
        $attrs = $this->_defaultInputAttributes($var, $prop, $attrs);
        $options = array_key_pop($attrs, 'options', array());
        $empty = array_key_pop($attrs, 'empty', false);
        $value = array_key_pop($attrs, 'value');

        # 开始生成HTML值。
        $html = '<select' . array_html_attrs($attrs) . '>';
        if ($empty) $html .= "<option>$empty</option>";

        # 根据数组生成选项列表。
        foreach ($options as $key => $title) {
            if ($key == $value) {
                $html .= "<option value=\"$key\" selected>$title</option>";
            } else {
                $html .= "<option value=\"$key\">$title</option>";
            }
        }

        return $html . '</select>';
    }

    /**
     * 根据$field和$attrs生成帮助信息。
     *
     * @param string $field 字段值。
     * @param mixed  $attrs 属性集，非数组表示为提示信息。
     *
     * @return string
     */
    public function help($field, $attrs = array()) {
        list($var, $prop) = $this->_splitField($field);

        # 如果$attrs为非数组，表示指定为提示信息。
        if (!is_array($attrs)) {
            $attrs = array(self::OPT_TITLE => $attrs);
        }

        # 处理默认参数。
        $elem = array_key_pop($attrs, 'elem', 'em');
        $title = array_key_pop($attrs, self::OPT_TITLE, '');

        $attrs['id'] = $this->_defaultFieldID($var, $prop, 'help');
        $attrs['data-help'] = $title;

        if ($error = $this->_defaultFieldError($var, $prop)) {
            array_join_str($attrs, 'class', $this->error);
            $title = $error;
        }

        return '<' . $elem . array_html_attrs($attrs) . '>' . $title . '</' . $elem . '>';
    }

    /**
     * 根据$attrs附加默认选项。
     *
     * @param array $attrs 当前选项
     *
     * @return array
     */
    protected function _defaultFormAttributes(array $attrs) {
        # 如果没有指定活动。
        if (!isset($attrs['action'])) {
            $attrs['action'] = Router::url(array(Router::FULL_URL => true));
        }

        # 如果没有指定方法选项。
        if (!isset($attrs['method'])) {
            $attrs['method'] = 'POST';
            $attrs[SKIHAT_PARAM_METHOD] = 'POST';
        } else if ($attrs['method'] != 'GET') {
            $attrs[SKIHAT_PARAM_METHOD] = $attrs['method'];
            $attrs['method'] = 'POST';
        }

        # 如果没有指定访问字符集。
        if (!isset($attrs['accept-charset'])) {
            $attrs['accept-charset'] = 'UTF-8';
        }

        return $attrs;
    }

    /**
     * 根据$field、$type和$attrs生成录入框。
     *
     * @param string $field 字段值。
     * @param string $type  录入框标志。
     * @param array  $attrs 录入框选项。
     *
     * @return string
     */
    protected function _inputTag($field, $type, $attrs = array()) {
        list($var, $prop) = $this->_splitField($field);

        $attrs = $this->_defaultInputAttributes($var, $prop, $attrs);
        if (!isset($attrs['value']) && ($value = $this->_defaultFieldValue($var, $prop)) !== null) {
            $attrs['value'] = $value;
        }

        # 如果设置有值，并且指定有格式器。
        if (($formatter = array_key_pop($attrs, self::OPT_FORMATTER)) && isset($attrs['value'])) {
            $attrs['value'] = call_user_func($formatter, $attrs['value']);
        }

        return '<input type="' . $type . '"' . array_html_attrs($attrs) . ' />';
    }

    /**
     * 根据$field分割出变量名称和属性。
     *
     * @param string $field 字段值。
     *
     * @return array (viewVar,prop);
     */
    protected function _splitField($field) {
        return explode(self::FIELD_SEPARATOR, $field);
    }

    /**
     * 根据$var、$prop和$attrs生成默认录入选项参数。
     *
     * @param string $var   变量名称。
     * @param string $prop  属性名称。
     * @param mixed  $attrs 选项。
     *
     * @return array
     */
    protected function _defaultInputAttributes($var, $prop, $attrs) {
        # 如果选项为非数组，则表示选项指定为值。
        if (!is_array($attrs)) $attrs = array('value' => $attrs);
        if (!isset($attrs['id'])) $attrs['id'] = $this->_defaultFieldID($var, $prop);
        if (!isset($attrs['name'])) $attrs['name'] = $this->_defaultFieldName($var, $prop);

        # 如果包含有错误消息，则指定错误class。
        if ($error = $this->_defaultFieldError($var, $prop)) {
            array_join_str($attrs, 'class', $this->error);
        }

        return $attrs;
    }

    /**
     * 根据$var、$prop和$extra生成ID名称。
     *
     * @param string      $var   变量名称。
     * @param string      $prop  属性名称。
     * @param string|bool $extra 额外名称。
     *
     * @return string
     */
    protected function _defaultFieldID($var, $prop, $extra = false) {
        if ($extra) {
            return sprintf('%s-%s-%s', $var, $prop, $extra);
        } else return sprintf('%s-%s', $var, $prop);
    }

    /**
     * 根据$var和$prop生成默认字段名称。
     *
     * @param string $var  变量名称。
     * @param string $prop 属性名称。
     *
     * @return string
     */
    protected function _defaultFieldName($var, $prop) {
        return sprintf('%s[%s]', $var, $prop);
    }

    /**
     * 根据$var和$prop生成默认标题。
     *
     * @param string $var  变量名称。
     * @param string $prop 属性名称。
     *
     * @return string
     */
    protected function _defaultFieldTitle($var, $prop) {
        return $var . ' ' . $prop;
    }

    /**
     * 根据$var和$prop获取字段的默认值。
     *
     * @param string $var  变量名称。
     * @param string $prop 属性名称。
     *
     * @return mixed
     */
    protected function _defaultFieldValue($var, $prop) {
        return isset($this->_context[$var][$prop]) ? $this->_context[$var][$prop] : null;
    }

    /**
     * $var和$prop返回错误消息。
     *
     * @param string $var  变量名称。
     * @param string $prop 属性名称。
     *
     * @return mixed
     */
    protected function _defaultFieldError($var, $prop) {
        # 判断当前变量是否存在，并且变量是否包含error方法。
        if (isset($this->_context[$var]) && method_exists($this->_context[$var], 'error')) {
            return call_user_func(array($this->_context[$var], 'error'), $prop);
        }

        return false;
    }
}

/**
 * FormBuilder的标记直接输出版本。
 *
 * @package skihat.views.__helpers
 */
class Form extends FormBuilder {
    /**
     * @see FormBuilder::begin。
     */
    public function begin(array $attrs = array()) {
        echo parent::begin($attrs);
    }

    /**
     * @see FormBuilder::end。
     */
    public function end() {
        echo parent::end();
    }

    /**
     * @see FormBuilder::label
     */
    public function label($field, $attrs = array()) {
        echo parent::label($field, $attrs);
    }

    /**
     * @see FormBuilder::hidden
     */
    public function hidden($field, $attrs = array()) {
        echo parent::hidden($field, $attrs);
    }

    /**
     * @see FormBuilder::text
     */
    public function text($field, $attrs = array()) {
        echo parent::text($field, $attrs);
    }

    /**
     * @see FormBuilder::email
     */
    public function email($field, $attrs = array()) {
        echo parent::email($field, $attrs);
    }

    /**
     * @see FormBuilder::password
     */
    public function password($field, $attrs = array()) {
        echo parent::password($field, $attrs);
    }

    /**
     * @see FormBuilder::file
     */
    public function file($field, $attrs = array()) {
        echo parent::file($field, $attrs);
    }

    /**
     * @see FormBuilder::url
     */
    public function url($field, $attrs = array()) {
        echo parent::url($field, $attrs);
    }

    /**
     * @see FormBuilder::number
     */
    public function number($field, $attrs = array()) {
        echo parent::number($field, $attrs);
    }

    /**
     * @see FormBuilder::range
     */
    public function range($field, $attrs = array()) {
        echo parent::range($field, $attrs);
    }

    /**
     * @see FormBuilder::datetime
     */
    public function datetime($field, $attrs = array()) {
        echo parent::datetime($field, $attrs);
    }

    /**
     * @see FormBuilder::date
     */
    public function date($field, $attrs = array()) {
        echo parent::date($field, $attrs);
    }

    /**
     * @see FormBuilder::time
     */
    public function time($field, $attrs = array()) {
        echo parent::time($field, $attrs);
    }

    /**
     * @see FormBuilder::month
     */
    public function month($field, $attrs = array()) {
        echo parent::month($field, $attrs);
    }

    /**
     * @see FormBuilder::week
     */
    public function week($field, $attrs = array()) {
        echo parent::week($field, $attrs);
    }

    /**
     * @see FormBuilder::textarea
     */
    public function textarea($field, $attrs = array()) {
        echo parent::textarea($field, $attrs);
    }

    /**
     * @see FormBuilder::checkbox
     */
    public function checkbox($field, $attrs = array()) {
        echo parent::checkbox($field, $attrs);
    }

    /**
     * @see FormBuilder::radio
     */
    public function radio($field, $attrs = array()) {
        echo parent::radio($field, $attrs);
    }

    /**
     * @see FormBuilder::select
     */
    public function select($field, $attrs = array()) {
        echo parent::select($field, $attrs);
    }

    /**
     * @see FormBuilder::help
     */
    public function help($field, $attrs = array()) {
        echo parent::help($field, $attrs);
    }
}
