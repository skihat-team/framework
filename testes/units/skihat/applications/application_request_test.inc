<?php
/**
 * application_request_test.inc 提供应用请求单元测试类声明。
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
Skihat::import('applications.application_request', SKIHAT_PATH_LIBRARY);

/**
 * 应用请求单元测试类，提供针对应用请求的单元测试服务。
 *
 * @package skihat.applications
 */
class ApplicationRequestTest extends PHPUnit_Framework_TestCase {

    /**
     * 测试查询参数。
     */
    public function testQuery() {
        $request = new ApplicationRequest();
        $request->initialize(array('name' => 'example', 'age' => 18));

        $this->assertEquals('example', $request->query('name'));
        $this->assertEquals(18, $request->query('age'));
    }

    /**
     * 测试查询参数，默认值。
     */
    public function testQueryForDefault() {
        $request = new ApplicationRequest();

        $this->assertFalse($request->query('name'));
        $this->assertEquals('example', $request->query('name', 'example'));
    }

    /**
     * 测试表单参数。
     */
    public function testForm() {
        $request = new ApplicationRequest();
        $request->initialize(array(), array('name' => 'example', 'age' => 18));

        $this->assertEquals('example', $request->form('name'));
        $this->assertEquals(18, $request->form('age'));
    }

    /**
     * 测试表单参数，默认值。
     */
    public function testFormForDefault() {
        $request = new ApplicationRequest();

        $this->assertFalse($request->form('name'));
        $this->assertEquals('example', $request->form('name', 'example'));
    }

    /**
     * 测试文件参数。
     */
    public function testFile() {
        $request = new ApplicationRequest();
        $request->initialize(array(), array(), array('file' => array('name' => 'example.gif', 'size' => 1000)));

        $file = $request->file('file');
        $this->assertEquals('example.gif', $file['name']);
        $this->assertEquals(1000, $file['size']);
    }

    /**
     * 测试文件参数，数组文件。
     */
    public function testFileForArray() {
        $request = new ApplicationRequest();
        $request->initialize(array(), array(), array('file' => array(
            'name' => array('example.gif', 'ab.gif'),
            'size' => array(1000, 2000),
            'type' => array('gif', 'gif'),
            'tmp_name' => array('1.tmp', '2.tmp'),
            'error' => array(0, 0))
        ));

        $files = $request->file('file');

        $this->assertEquals(2, count($files));
        $this->assertEquals('example.gif', $files[0]['name']);
        $this->assertEquals('ab.gif', $files[1]['name']);
    }

    /**
     * 测试文件参数，文件不存在。
     */
    public function testFileForNotExists() {
        $request = new ApplicationRequest();

        $this->assertFalse($request->file('not-exist'));
    }
}