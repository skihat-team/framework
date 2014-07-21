<?php
/**
 * transaction.inc 提供事务处理组件接口声明。
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

/**
 * 事务异常，当事务组件发生错误时引发本异常。
 *
 * @package skihat.kernels.transactions
 */
class TransactionException extends SkihatException {
}

/**
 * 事务观察者，提供事务处理所需的基本支持。
 *
 * @package skihat.kernels.transactions
 */
interface ITransactionObserver {
    /**
     * 开启事务支持，如果开启失败，使用异常表示失败。
     */
    function begin();

    /**
     * 提交事务，如果提交失败，请使用异常表示失败。
     *
     * @注意：所有的提交事务不应当产生任何异常，否则有可能造成事务没有同步的问题。
     */
    function commit();

    /**
     * 回滚事务，允许在没有开启的情况下执行回滚。
     */
    function rollback();
}

/**
 * 事务接口类，提供统一事务处理接口。
 *
 * @more
 *  Transaction类作为Skihat的事务处理接口，具有以下特性：
 *  <ul>
 *      <li>1：基于异常的事务处理原则，即事务范围内的错误都必须通过异常来处理。</li>
 *      <li>2：支持单一事务，不支持分布式事务。</li>
 *      <li>3：事务应当由Service或Controller内部使用。</li>
 *  </li>
 *
 * @example
 *  <pre>
 *      try {
 *          Transaction::begin();
 *
 *          $member = new Member(1);
 *          $member->name = 'new name';
 *          $member->update();
 *
 *          Transaction::commit();
 *      } catch (Exception $ex) {
 *          Transaction::rollback();
 *      }
 *  </pre>
 *
 *
 * @package skihat.kernels.transactions
 */
abstract class Transaction {
    /**
     * @const string 事务调试名称。
     */
    const DEBUG_NAME = 'kernels/transactions';

    /**
     * 将$observer注册到事务组件，注册成功返回true，注册失败返回false。
     *
     * @param ITransactionObserver $observer 事务观察者。
     *
     * @return bool
     */
    public static function register(ITransactionObserver $observer) {
        # 如果注册已有的观察者。
        if (self::hasRegister($observer)) {
            trigger_error('Transaction::register repeat register observer.' . $observer);
            return false;
        }

        array_push(self::$_observers, $observer);
        if (self::$_begin) $observer->begin();

        return true;
    }

    /**
     * 根据$observer参数，注销一个已注册的事务观察者，成功返回true，失败返回false。
     *
     * @param ITransactionObserver $observer 事务观察者。
     *
     * @return bool
     */
    public static function unRegister(ITransactionObserver $observer) {
        # 如果在数组中包含指定值，删除并返回。
        if (($key = array_search($observer, self::$_observers)) !== false) {
            unset(self::$_observers[$key]);
            return true;
        }

        return false;
    }

    /**
     * 返回一个布尔值，表示当前事务组件中是否包含$observer的注册，有返回true，没有返回false。
     *
     * @param ITransactionObserver $observer 事务观察者。
     *
     * @return bool
     */
    public static function hasRegister(ITransactionObserver $observer) {
        return in_array($observer, self::$_observers);
    }

    /**
     * 返回一个布尔值，表示是否正处理于事务中。
     *
     * @return bool
     */
    public static function inTransaction() {
        return self::$_begin;
    }

    /**
     * 开启事务，所有已注册到组件中的观察者都将执行事务。
     */
    public static function begin() {
        # 如果重复开启事务组件。
        if (self::$_begin) {
            trigger_error('Transaction:begin repeat.');
            return false;
        }

        # 执行观察者开启事务处理。
        foreach (self::$_observers as $observer) {
            $observer->begin();
        }

        return self::$_begin = true;
    }

    /**
     * 提交已开启的事务，成功返回true，失败返回false。
     */
    public static function commit() {
        # 如果事务还没有开启。
        if (!self::$_begin) {
            trigger_error('Transaction:commit transaction not begin.');
            return false;
        }

        foreach (self::$_observers as $observer) {
            $observer->commit();
        }

        self::$_begin = false;
        return true;
    }

    /**
     * 回滚已处理的操作。
     */
    public static function rollback() {
        # 如果事务还没有开启。
        if (!self::$_begin) {
            trigger_error('Transaction:rollback transaction not begin.');
            return false;
        }

        self::$_begin = false;
        foreach (self::$_observers as $observer) {
            $observer->rollback();
        }

        return true;
    }

    /**
     * @var array 已注册的观察器集合。
     */
    protected static $_observers = array();

    /**
     * @var bool 是否已经开启事务处理。
     */
    protected static $_begin = false;
}