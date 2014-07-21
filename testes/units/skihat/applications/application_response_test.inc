<?php
/**
 * application_response_test.inc 提供应用响应单元测试类声明。
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

# 引用测试启动文件。
require_once(dirname(dirname(dirname(__FILE__))) . '/startup.inc');

# 引用被测试类。
Skihat::import('applications.application_response', SKIHAT_PATH_LIBRARY);

/**
 * 应用响应单元测试类，提供对应用响应类的单元测试服务。
 *
 * @package skihat.applications
 */
class ApplicationResponseTest extends PHPUnit_Framework_TestCase {

    /**
     * 测试自定义响应头。
     */
    public function testHeaders() {
        $response = new ApplicationResponse();
        $this->assertEmpty($response->allHeaders());

        $response->header('custom', 12);
        $response->header('custom1', 'hello world');

        $headers = $response->allHeaders();
        $this->assertEquals(12, $headers['custom']);
        $this->assertEquals('hello world', $headers['custom1']);
    }

    /**
     * 测试状态码。
     */
    public function testStatusCode() {
        $response = new ApplicationResponse();

        $this->assertEquals(200, $response->statusCode());
        $this->assertEquals(404, $response->statusCode(404));
        $this->assertEquals(404, $response->statusCode(404));
    }

    /**
     * 测试内容类型。
     */
    public function testContentType() {
        $response = new ApplicationResponse();

        $this->assertEquals('text/html;charset=utf-8', $response->contentType());
        $this->assertEquals('text/json', $response->contentType('text/json'));
        $this->assertEquals('text/json', $response->contentType());
    }

    /**
     * 测试响应内容。
     */
    public function testContentBody() {
        $response = new ApplicationResponse();

        $this->assertNull($response->contentBody());
        $this->assertEquals('hello', $response->contentBody('hello'));
        $this->assertEquals('hello', $response->contentBody());
    }
}