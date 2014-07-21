<?php
/**
 * application_response.inc 提供应用响应声明。
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
 * 应用响应类，提供应用程序的响应支持。
 *
 * @package skihat.applications
 */
class ApplicationResponse {
    /**
     * 根据$name和$value参数，设置自定义响应头，如果$name已存在则覆盖已有的值。
     *
     * @param string $name  参数名称。
     * @param mixed  $value 参数值。
     */
    public function header($name, $value) {
        $this->_headers[$name] = $value;
    }

    /**
     * 返回所有自定义响应头。
     *
     * @return array
     */
    public function allHeaders() {
        return $this->_headers;
    }

    /**
     * 根据$minute和$lasted开启客户端缓存信息。
     *
     * @param int  $minute 缓存分钟数，默认为30分钟。
     * @param bool $lasted 最后修改时间，默认为false。
     */
    public function enableCache($minute = 30, $lasted = false) {
        # 设置缓存值。
        $timestamp = $minute * 60;
        if (!$lasted) $lasted = time();

        # 设置缓存控制值。
        $this->_cacheControl = array(
            'cache-control' => "max-age={$timestamp}",
            'Pragma' => 'public',
            'Last-Modified' => gmdate('D, d M Y H:i:s', $lasted) . ' GMT',
            'Expires' => gmdate('D, d M Y H:i:s', time() + $timestamp) . ' GMT');
    }

    /**
     * 禁止客户端缓存。
     */
    public function disableCache() {
        $this->_cacheControl = array('cache-control' => 'no-cache', 'Pragma' => 'no-cache');
    }

    /**
     * 获取或设置响应状态码，如果$statusCode为非空值，则表示仅仅获取状态码。
     *
     * @param mixed $statusCode HTTP 状态码。
     *
     * @return mixed
     */
    public function statusCode($statusCode = null) {
        # 如果状态码为非false值，设置状态码。
        if ($statusCode) {
            $this->_statusCode = $statusCode;
        }

        return $this->_statusCode;
    }

    /**
     * 获取或设置响应内容类型，如果$contentType为非空值，则表示仅仅获取内容类型。
     *
     * @param string $contentType 内容类型。
     *
     * @return string
     */
    public function contentType($contentType = null) {
        # 如果内容类型为非false值，设置内容类型。
        if ($contentType) {
            $this->_contentType = $contentType;
        }

        return $this->_contentType;
    }

    /**
     * 白
     *
     * @param mixed $contentBody 内容主体信息。
     *
     * @return mixed
     */
    public function contentBody($contentBody = null) {
        # 如果内容主体为非空值，设置新的内容信息。
        if ($contentBody) {
            $this->_contentBody = $contentBody;
        }

        return $this->_contentBody;
    }

    /**
     * 将响应信息输出到客房端。
     */
    public function render() {
        $this->_printHeaders();
        $this->_printContent();
    }

    /**
     * 根据$response执行内容复制。
     *
     * @param ApplicationResponse $response 另一个响应头。
     */
    public function copy(ApplicationResponse $response) {
        $this->_headers = $response->_headers;

        $this->_statusCode = $response->_statusCode;
        $this->_cacheControl = $response->_cacheControl;
        $this->_contentType = $response->_contentType;
        $this->_contentBody = $response->_contentBody;
    }

    /**
     * 打印响应头。
     */
    protected function _printHeaders() {
        # 输出状态码。
        if ($statusCode = $this->_statusCode) {
            header('HTTP/1.1 ' . $statusCode);
            header('status:' . $statusCode);
        }

        # 输出内容类型。
        if ($contentType = $this->_contentType) {
            header('Content-type:' . $contentType);
        }

        # 输出缓存控制。
        if ($cacheControl = $this->_cacheControl) {
            $this->_printHeader('Cache-Control', $cacheControl);
        }

        # 输出自定头。
        foreach ($this->_headers as $name => $value) {
            $this->_printHeader($name, $value);
        }
    }

    /**
     * 打印内容。
     */
    protected function _printContent() {
        echo $this->_contentBody;
    }

    /**
     * 根据$name和$value参数，打印输出响应头。
     *
     * @param string $name  头名称。
     * @param mixed  $value 响应值。
     */
    protected function _printHeader($name, $value) {
        if (is_array($value)) {
            foreach ($value as $name => $val) {
                header($name . ':' . $val);
            }
        } else {
            header($name . ':' . $value);
        }
    }

    /**
     * @var int 响应状态码。
     */
    protected $_statusCode = 200;

    /**
     * @var string 响应内容类型。
     */
    protected $_contentType = 'text/html;charset=utf-8';

    /**
     * @var mixed 响应内容主体。
     */
    protected $_contentBody = NULL;

    /**
     * @var bool|mixed 响应缓存控制。
     */
    protected $_cacheControl = false;

    /**
     * @var array 自定义响应头。
     */
    protected $_headers = array();
}