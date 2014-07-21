<?php

/**
 * transaction_test.inc 提供事务处理单元测试声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.transactions
 * @version   1.0
 *
 * @created   2012-07-09
 * @modified  2013-09-18
 */

# 引用测试启动文件。
require_once(dirname(dirname(dirname(dirname(__FILE__)))) . '/startup.inc');

# 引用被测试类。
Skihat::import('kernels.transactions.transaction', SKIHAT_PATH_LIBRARY);

/**
 * 事务组合单元测试，提供对事务组合件的单元测试服务。
 *
 * @package skihat.kernels.transactions
 */
class TransactionTest extends PHPUnit_Framework_TestCase {

    /**
     * 测试注册观察者。
     */
    public function testRegister() {
        $default = new DefaultTransactionObserver();

        $this->assertTrue(TransactionMock::register($default));
        $this->assertTrue(TransactionMock::hasRegister($default));
    }

    /**
     * 测试注册观察者，重重注册。
     */
    public function testRegisterForRepeat() {
        $default = new DefaultTransactionObserver();

        $this->assertTrue(TransactionMock::register($default));
        $this->assertFalse(TransactionMock::register($default));
    }

    /**
     * 测试注销观察者。
     */
    public function testUnRegister() {
        $default = new DefaultTransactionObserver();

        $this->assertTrue(TransactionMock::register($default));
        $this->assertTrue(TransactionMock::unRegister($default));

        $this->assertFalse(TransactionMock::hasRegister($default));
    }

    /**
     * 测试注销观察者，注销没有注册的观察者。
     */
    public function testUnRegisterForNotRegister() {
        $default = new DefaultTransactionObserver();
        $this->assertFalse(TransactionMock::unRegister($default));
    }

    /**
     * 测试提交事务.
     */
    public function testCommit() {
        $default = new DefaultTransactionObserver();
        TransactionMock::register($default);

        try {
            TransactionMock::begin();
            TransactionMock::commit();
        } catch (Exception $ex) {
            TransactionMock::rollback();
        }

        $this->assertTrue($default->callBegin);
        $this->assertTrue($default->callCommit);
    }

    /**
     * 测试提交事务，提交回滚。
     */
    public function testCommitForRollback() {
        $default = new DefaultTransactionObserver();
        TransactionMock::register($default);

        try {
            TransactionMock::begin();
            if (true) {
                throw new Exception('error');
            }

            TransactionMock::commit();
        } catch (Exception $ex) {
            TransactionMock::rollback();
        }

        $this->assertTrue($default->callBegin);
        $this->assertFalse($default->callCommit);
        $this->assertTrue($default->callRollback);
    }

    /**
     * 测试提交事务，发生异常情况。
     */
    public function testCommitForException() {
        $default = new DefaultTransactionObserver();
        $example = new DefaultTransactionObserver();
        $default->throws = true;

        TransactionMock::register($default);
        TransactionMock::register($example);

        try {
            TransactionMock::begin();
            TransactionMock::commit();
        } catch (Exception $ex) {
            TransactionMock::rollback();
        }

        $this->assertTrue($default->callBegin);
        $this->assertFalse($default->callCommit);
        $this->assertTrue($default->callRollback);

        $this->assertTrue($example->callBegin);
        $this->assertFalse($example->callCommit);
        $this->assertTrue($example->callRollback);
    }

    /**
     * 测试注册观察者，事务已启动。
     */
    public function testRegisterForBegin() {
        $default = new DefaultTransactionObserver();

        try {
            TransactionMock::begin();
            TransactionMock::register($default);
            TransactionMock::commit();
        } catch (Exception $ex) {
            TransactionMock::rollback();
        }

        $this->assertTrue($default->callBegin);
        $this->assertTrue($default->callCommit);
        $this->assertFalse($default->callRollback);
    }

    /**
     * @see PHPUnit_Framework_TestCase::setUp
     */
    public function setUp() {
        TransactionMock::reset();
        set_error_handler(array($this, 'error'));
    }

    /**
     * 自定义错误处理@see set_error_handler
     */
    public function error($errno, $errstr, $errfile, $errline) {
    }
}

/**
 * 默认事务观察者，提供事务组件单元测试支持。
 *
 * @package skihat.kernels.transactions
 */
class DefaultTransactionObserver implements ITransactionObserver {
    public $callBegin = false;
    public $callCommit = false;
    public $callRollback = false;
    public $throws = false;

    public function begin() {
        $this->callBegin = true;
    }

    public function commit() {
        if ($this->throws) {
            throw new TransactionException('fail');
        }
        $this->callCommit = true;
    }

    public function rollback() {
        $this->callRollback = true;
    }
}

/**
 * 实例事务观察者，提供事务组年单元测试支持。
 */
class TransactionMock extends Transaction {
    public static function reset() {
        self::$_observers = array();
        self::$_begin = false;
    }
}