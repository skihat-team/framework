<?php
/**
 * application_request.inc 提供应用请求类声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.applications
 * @version   1.0
 *
 * @created   2012-07-23
 * @modified  2013-08-11
 */

/**
 * 应用请求类，封装客户的请求信息。
 *
 * @package skihat.applications
 */
class ApplicationRequest {
    /**
     * 根据$params，$forms和$files初始化应用请求类。
     *
     * @param array $params 请求$_GET参数。
     * @param array $forms  请求$_POST参数。
     * @param array $files  请求$_FILES参数。
     */
    public function initialize(array $params, array $forms = array(), array  $files = array()) {
        # 初始化表单和请求参数信息。
        $this->_forms = $forms;
        $this->_params = $params;

        # 如果请求上传文件信息不为空。
        if (!empty($files)) {
            $this->_files = $this->_adjustFiles($files);
        }
    }

    /**
     * 根据$name和$default获取查询字符串，如果指定$name不存在则返回$default值。
     *
     * @param string $name    GET参数名称。
     * @param bool   $default 返回默认值，默认为false。
     *
     * @return mixed
     */
    public function query($name, $default = false) {
        return isset($this->_params[$name]) ? $this->_params[$name] : $default;
    }

    /**
     * 返回当前应用请求的所有请求参数。
     *
     * @return array
     */
    public function queryAll() {
        return $this->_params;
    }

    /**
     * 根据$name和$default获取表单参数值，如果指定$name不存在则返回$default值。
     *
     * @param string $name    POST参数名称。
     * @param bool   $default 默认值，默认为false。
     *
     * @return mixed
     */
    public function form($name, $default = false) {
        return isset($this->_forms[$name]) ? $this->_forms[$name] : $default;
    }

    /**
     * 根据$name获取表单上传文件值，如果指定$name的文件不存在则返回false。
     *
     * @param string $name FILES参数名称。
     *
     * @return mixed
     */
    public function file($name) {
        return isset($this->_files[$name]) ? $this->_files[$name] : false;
    }

    /**
     * 返回请求械块信息，如果包参数不存在，则返回$default值。
     *
     * @param mixed $default 默认值，默认为SKIHAT_PARAM_DEFAULT_MODULE.
     *
     * @return string|bool
     */
    public function module($default = SKIHAT_PARAM_DEFAULT_MODULE) {
        return $this->query(SKIHAT_PARAM_MODULE, $default);
    }

    /**
     * 返回请求包信息，如果包参数不存在，则返回$default值。
     *
     * @param mixed $default 默认值q，默认为SKIHAT_PARAM_DEFAULT_PACKAGE.
     *
     * @return mixed
     */
    public function package($default = SKIHAT_PARAM_DEFAULT_PACKAGE) {
        return $this->query(SKIHAT_PARAM_PACKAGE, $default);
    }

    /**
     * 返回请求控制器信息，如果控制参数不存在，则返回$default值。
     *
     * @param mixed $default 默认值，默认为SKIHAT_PARAM_DEFAULT_CONTROLLER.
     *
     * @return mixed
     */
    public function controller($default = SKIHAT_PARAM_DEFAULT_CONTROLLER) {
        return $this->query(SKIHAT_PARAM_CONTROLLER, $default);
    }

    /**
     * 返回请求活动信息，如果活动参数不存在，则返回$default值。
     *
     * @param mixed $default 默认值，默认为SKIHAT_PARAM_DEFAULT_ACTION.
     *
     * @return mixed
     */
    public function action($default = SKIHAT_PARAM_DEFAULT_ACTION) {
        return $this->query(SKIHAT_PARAM_ACTION, $default);
    }

    /**
     * 返回请求的格式信息，如果格式参数不存在，则返回$default值。
     *
     * @param mixed $default 默认值，默认为SKIHAT_PARAM_DEFAULT_FMT.
     *
     * @return mixed
     */
    public function fmt($default = SKIHAT_PARAM_DEFAULT_FMT) {
        return $this->query(SKIHAT_PARAM_FMT, $default);
    }

    /**
     * 根据$name参数，获取请求头信息，如果$name指定值不存在，返回$default值。
     *
     * @param string $name    名称。
     * @param bool   $default 默认值，默认为false。
     *
     * @return mixed
     */
    public static function header($name, $default = false) {
        $fulName = 'HTTP_' . strtolower($name);
        return empty($_SERVER[$fulName]) ? $_SERVER[$fulName] : $default;
    }

    /**
     * 将上传$files文件格式，调整为数组文件格式。
     *
     * @example
     *  <pre>
     *      $files = $this->_adjustFiles(array('users' => array(
     *                                                  'name' => array('thumb'=>'1.gif','thumb1'=>'2.gif'),
     *                                                  'size' => array('thumb' =>1,'thumb1' => 2),
     *                                                  ...)));
     *      $files = {'users' => array('thumb' => array('name' => '1.gif','size'=> 1 ),
     *                                 'thumb1'=> array('name' => '2.gif','size => 2)
     *  </pre>
     *
     * @param array $files 文件参数。
     *
     * @return array
     */
    protected function _adjustFiles(array $files) {
        # 获取全部上传文件参数。
        foreach ($files as $name => $file) {
            if (is_array($file['name'])) {
                $files[$name] = array();
                $fields = array_keys($file['name']);

                foreach ($fields as $field) {
                    $files[$name][$field] = array(
                        'name' => $file['name'][$field],
                        'type' => $file['type'][$field],
                        'size' => $file['size'][$field],
                        'error' => $file['error'][$field],
                        'tmp_name' => $file['tmp_name'][$field]
                    );
                }
            }
        }

        return $files;
    }

    /**
     * @var array 请求参数。
     */
    protected $_params = array();

    /**
     * @var array 表单参数。
     */
    protected $_forms = array();

    /**
     * @var array 文件参数。
     */
    protected $_files = array();
}