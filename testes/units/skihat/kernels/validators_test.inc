<?php
/**
 * validators_test.inc 验证器方法单元测试文件。
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
require_once(dirname(dirname(dirname(__FILE__))) . '/startup.inc');

# 导入被测试类
Skihat::import('kernels.validators', SKIHAT_PATH_LIBRARY);

/**
 * 验证器单元测试类，提供验证模块的单元测试服务。
 *
 * @package skihat.kernels
 */
class ValidatorsTest extends PHPUnit_Framework_TestCase {

    /**
     * 测试非空验证器。
     */
    public function testRequiredValidator() {
        $input = array('name' => 'skihat', 'fullName' => 'Skihat PHP Framework', 'allow' => false, 'age' => 0);

        $this->assertTrue(required_validator($input, 'name'));
        $this->assertTrue(required_validator($input, 'fullName'));
        $this->assertFalse(required_validator($input, 'allow'));
        $this->assertFalse(required_validator($input, 'age'));
    }

    /**
     * 测试数值验证器。
     */
    public function testNumericValidator() {
        $input = array('age' => 12, 'price' => 10.2, 'name' => '13dg', 'total' => '123');

        $this->assertTrue(numeric_validator($input, 'age'));
        $this->assertTrue(numeric_validator($input, 'price'));
        $this->assertTrue(numeric_validator($input, 'total'));
        $this->assertFalse(numeric_validator($input, 'name'));
    }

    /**
     * 测试数值验证器，空值。
     */
    public function testNumericValidatorForEmpty() {
        $input = array('age' => 0, 'price' => 0, 'allow' => false, 'name' => '');

        $this->assertTrue(numeric_validator($input, 'age'));
        $this->assertTrue(numeric_validator($input, 'allow'));
        $this->assertTrue(numeric_validator($input, 'name'));
        $this->assertTrue(numeric_validator($input, 'not-exists'));
    }

    /**
     * 测试电话验证器。
     */
    public function testTelephoneValidator() {
        $input = array('tel' => '023-65730912', 'tel1' => '8898-6612341', 'tel2' => '400-023-98761', 'iphone' => '18712312341', 'htc' => '1319193123');

        $this->assertTrue(telephone_validator($input, 'tel'));
        $this->assertTrue(telephone_validator($input, 'tel1'));
        $this->assertTrue(telephone_validator($input, 'tel2'));
        $this->assertTrue(telephone_validator($input, 'iphone'));
        $this->assertFalse(telephone_validator($input, 'htc'));
    }

    /**
     * 测试电话验证器，空值。
     */
    public function testTelephoneValidatorForEmpty() {
        $input = array('age' => 0, 'price' => 0, 'allow' => false, 'name' => '');

        $this->assertTrue(telephone_validator($input, 'age'));
        $this->assertTrue(telephone_validator($input, 'allow'));
        $this->assertTrue(telephone_validator($input, 'name'));
        $this->assertTrue(telephone_validator($input, 'not-exists'));
    }

    /**
     * 测试邮件验证器。
     */
    public function testEmailValidator() {
        $input = array('email' => 'skihat.framework@google.com', 'email1' => 'test.vip@sohu.com', 'email2' => 'test-vip@vip.sohu.com', 'err' => 'abc@13');

        $this->assertTrue(email_validator($input, 'email'));
        $this->assertTrue(email_validator($input, 'email1'));
        $this->assertTrue(email_validator($input, 'email2'));
        $this->assertFalse(email_validator($input, 'err'));
    }

    /**
     * 测试邮箱验证器，空值。
     */
    public function testEmailValidatorForEmpty() {
        $input = array('age' => 0, 'price' => 0, 'allow' => false, 'name' => '');

        $this->assertTrue(email_validator($input, 'age'));
        $this->assertTrue(email_validator($input, 'allow'));
        $this->assertTrue(email_validator($input, 'name'));
        $this->assertTrue(email_validator($input, 'not-exists'));
    }

    /**
     * 测试URL验证器。
     */
    public function testUrlValidator() {
        $input = array('sina' => 'http://www.sina.com',
            'twitter' => 'https://twitter.com',
            'skihat' => 'http://admins.example.com/?controller=index',
            'news' => 'http://www.example.com/news.html',
            'err' => 'www.example.com');

        $this->assertTrue(url_validator($input, 'sina'));
        $this->assertTrue(url_validator($input, 'twitter'));
        $this->assertTrue(url_validator($input, 'skihat'));
        $this->assertTrue(url_validator($input, 'news'));
        $this->assertFalse(url_validator($input, 'err'));
    }

    /**
     * 测试URL验证器，空值。
     */
    public function testUrlValidatorForEmpty() {
        $input = array('age' => 0, 'price' => 0, 'allow' => false, 'name' => '');

        $this->assertTrue(url_validator($input, 'age'));
        $this->assertTrue(url_validator($input, 'allow'));
        $this->assertTrue(url_validator($input, 'name'));
        $this->assertTrue(url_validator($input, 'not-exists'));
    }

    /**
     * 测试IP验证器。
     */
    public function testIpValidator() {
        $input = array('ip' => '127.0.0.1', 'ip1' => '255.255.255.254', 'local' => 'localhost', 'err' => 'www.example.com');

        $this->assertTrue(ip_validator($input, 'ip'));
        $this->assertTrue(ip_validator($input, 'ip1'));
        $this->assertFalse(ip_validator($input, 'local'));
        $this->assertFalse(ip_validator($input, 'err'));
    }

    /**
     * 测试IP验证器，空值。
     */
    public function testIpValidatorForEmpty() {
        $input = array('age' => 0, 'price' => 0, 'allow' => false, 'name' => '');

        $this->assertTrue(ip_validator($input, 'age'));
        $this->assertTrue(ip_validator($input, 'allow'));
        $this->assertTrue(ip_validator($input, 'name'));
        $this->assertTrue(ip_validator($input, 'not-exists'));
    }

    /**
     * 测试最小字符串验证器。
     */
    public function testStrminValidator() {
        $input = array('name' => 'skihat', 'zhCN' => '中文内容');

        $this->assertTrue(strmin_validator($input, 'name', 6));
        $this->assertFalse(strmin_validator($input, 'name', 7));

        $this->assertTrue(strmin_validator($input, 'zhCN', 12));
        $this->assertFalse(strmin_validator($input, 'zhCN', 13));
    }

    /**
     * 测试最小字符串验证器，空值。
     */
    public function testStrminValidatorForEmpty() {
        $input = array('age' => 0, 'price' => 0, 'allow' => false, 'name' => '');

        $this->assertTrue(strmin_validator($input, 'age'));
        $this->assertTrue(strmin_validator($input, 'allow'));
        $this->assertTrue(strmin_validator($input, 'name'));
        $this->assertTrue(strmin_validator($input, 'not-exists'));
    }

    /**
     * 测试最大字符串验证器。
     */
    public function testStrmaxValidator() {
        $input = array('name' => 'skihat', 'zhCN' => '中文内容');

        $this->assertTrue(strmax_validator($input, 'name', 7));
        $this->assertTrue(strmax_validator($input, 'name', 6));
        $this->assertFalse(strmax_validator($input, 'name', 5));

        $this->assertTrue(strmax_validator($input, 'zhCN', 14));
        $this->assertTrue(strmax_validator($input, 'zhCN', 12));
        $this->assertFalse(strmax_validator($input, 'zhCN', 11));
    }

    /**
     * 测试最大字符串验证器，空值。
     */
    public function testStrmaxValidatorForEmpty() {
        $input = array('age' => 0, 'price' => 0, 'allow' => false, 'name' => '');

        $this->assertTrue(strmax_validator($input, 'age'));
        $this->assertTrue(strmax_validator($input, 'allow'));
        $this->assertTrue(strmax_validator($input, 'name'));
        $this->assertTrue(strmax_validator($input, 'not-exists'));
    }

    /**
     * 测试字符串长度范围验证器。
     */
    public function testStrlenValidator() {
        $input = array('name' => 'skihat', 'zhCN' => '中文内容');

        $this->assertTrue(strlen_validator($input, 'name', array(6, 10)));
        $this->assertTrue(strlen_validator($input, 'name', array(4, 10)));
        $this->assertTrue(strlen_validator($input, 'name', array(3, 6)));

        $this->assertFalse(strlen_validator($input, 'name', array(3, 4)));
        $this->assertFalse(strlen_validator($input, 'name', array(7, 10)));


        $this->assertTrue(strlen_validator($input, 'zhCN', array(12, 20)));
        $this->assertTrue(strlen_validator($input, 'zhCN', array(4, 20)));
        $this->assertTrue(strlen_validator($input, 'zhCN', array(4, 12)));

        $this->assertFalse(strlen_validator($input, 'zhCN', array(3, 4)));
        $this->assertFalse(strlen_validator($input, 'zhCN', array(14, 20)));
    }


    /**
     * 测试最大字符串验证器，空值。
     */
    public function testStrlenValidatorForEmpty() {
        $input = array('age' => 0, 'price' => 0, 'allow' => false, 'name' => '');

        $this->assertTrue(strlen_validator($input, 'age'));
        $this->assertTrue(strlen_validator($input, 'allow'));
        $this->assertTrue(strlen_validator($input, 'name'));
        $this->assertTrue(strlen_validator($input, 'not-exists'));
    }

    /**
     * 测试最小值范围。
     */
    public function testNumminValidator() {
        $input = array('age' => 18);

        $this->assertTrue(nummin_validator($input, 'age', 18));
        $this->assertTrue(nummin_validator($input, 'age', 17));
        $this->assertFalse(nummin_validator($input, 'age', 20));
    }

    /**
     * 测试最小值范围，空值。
     */
    public function testNumminValidatorForEmpty() {
        $input = array('age' => 0, 'price' => 0, 'allow' => false, 'name' => '');

        $this->assertTrue(nummin_validator($input, 'age'));
        $this->assertTrue(nummin_validator($input, 'allow'));
        $this->assertTrue(nummin_validator($input, 'name'));
        $this->assertTrue(nummin_validator($input, 'not-exists'));
    }

    /**
     * 测试最大值范围。
     */
    public function testNummaxValidator() {
        $input = array('age' => 18);

        $this->assertTrue(nummax_validator($input, 'age', 18));
        $this->assertTrue(nummax_validator($input, 'age', 20));
        $this->assertFalse(nummax_validator($input, 'age', 17));
    }

    /**
     * 测试最大值范围，空值。
     */
    public function testNummaxValidatorForEmpty() {
        $input = array('age' => 0, 'price' => 0, 'allow' => false, 'name' => '');

        $this->assertTrue(nummax_validator($input, 'age'));
        $this->assertTrue(nummax_validator($input, 'allow'));
        $this->assertTrue(nummax_validator($input, 'name'));
        $this->assertTrue(nummax_validator($input, 'not-exists'));
    }

    /**
     * 测试数值范围。
     */
    public function testNumrangeValidator() {
        $input = array('age' => 18);

        $this->assertTrue(numrang_validator($input, 'age', array(10, 18)));
        $this->assertTrue(numrang_validator($input, 'age', array(18, 20)));
        $this->assertTrue(numrang_validator($input, 'age', array(12, 20)));
        $this->assertFalse(numrang_validator($input, 'age', array(12, 16)));
        $this->assertFalse(numrang_validator($input, 'age', array(19, 30)));
    }

    /**
     * 测试数值范围，
     */
    public function testNumrangeForEmpty() {
        $input = array('age' => 0, 'price' => 0, 'allow' => false, 'name' => '');

        $this->assertTrue(numrang_validator($input, 'age'));
        $this->assertTrue(numrang_validator($input, 'allow'));
        $this->assertTrue(numrang_validator($input, 'name'));
        $this->assertTrue(numrang_validator($input, 'not-exists'));
    }

    /**
     * 测试字段比较验证器。
     */
    public function testCompareValidator() {
        $input = array('password' => '123321', 'confirm_password' => '123321', 'cpassword' => 'password');

        $this->assertTrue(compare_validator($input, 'password', 'confirm_password'));
        $this->assertFalse(compare_validator($input, 'password', 'cpassword'));
    }

    /**
     * 测试字段比较验证器，空值。
     */
    public function testCompareValidatorForEmpty() {
        $input = array('age' => 0, 'price' => 0, 'allow' => false, 'name' => '');

        $this->assertTrue(compare_validator($input, 'age', 'allow'));
        $this->assertTrue(compare_validator($input, 'name', 'allow'));
        $this->assertTrue(compare_validator($input, 'not-exists', 'allow'));
        $this->assertTrue(compare_validator($input, 'age', 'not-exists'));
    }

    /**
     * 测试枚举验证器。
     */
    public function testEnumValidator() {
        $input = array('name' => 'skihat');

        $this->assertTrue(enum_validator($input, 'name', array('skihat', 'php')));
        $this->assertTrue(enum_validator($input, 'name', 'skihat'));
        $this->assertFalse(enum_validator($input, 'name', array('php')));
    }

    /**
     * 测试枚举验证器，空值。
     */
    public function testEnumValidatorForEmpty() {
        $input = array('age' => 0, 'price' => 0, 'allow' => false, 'name' => '');

        $this->assertTrue(enum_validator($input, 'age', 'allow'));
        $this->assertTrue(enum_validator($input, 'name', 'allow'));
        $this->assertTrue(enum_validator($input, 'not-exists', 'allow'));
        $this->assertTrue(enum_validator($input, 'age', 'not-exists'));
    }

    /**
     * 测试验证正则验证器。
     */
    public function testRegexValidator() {
        $input = array('name' => 'skihat');

        $this->assertTrue(regex_validator($input, 'name', '/^[a-z]*$/'));
        $this->assertFalse(regex_validator($input, 'name', '/^[0-9]*$/'));
    }

    /**
     * 测试验证正则验证器，空值。
     */
    public function testRegexValidatorForEmpty() {
        $input = array('age' => 0, 'price' => 0, 'allow' => false, 'name' => '');

        $this->assertTrue(regex_validator($input, 'age', '/^[a-z]*$/'));
        $this->assertTrue(regex_validator($input, 'name', '/^[a-z]*$/'));
        $this->assertTrue(regex_validator($input, 'not-exists', '/^[a-z]*$/'));
        $this->assertTrue(regex_validator($input, 'age', '/^[a-z]*$/'));
    }

    /**
     * 测试执行验证器。
     */
    public function testValidateValidatorsForSingleValidator() {
        $success = validate_validators(array('name' => 'skihat'), array('name' => 'required'));
        $fail = validate_validators(array('name' => ''), array('name' => 'required'), array('name' => 'required'));

        $this->assertEmpty($success);
        $this->assertNotEmpty($fail);
        $this->assertEquals('required', $fail['name']);
    }

    /**
     * 测试执行验证器，多个验证器。
     */
    public function testValidateValidatorForMultiplyValidator() {
        $success = validate_validators(array('name' => 'skihat'), array('name' => array('required' => true, 'strmin' => 3)));
        $fail = validate_validators(array('name' => 'skihat'), array('name' => array('required' => true, 'strmin' => 10)), array('name' => array('strmin' => 'strmin')));

        $this->assertEmpty($success);
        $this->assertNotEmpty($fail);
        $this->assertEquals('strmin', $fail['name']);
    }
}

