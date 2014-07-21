<?php
/**
 * startup.inc 提供SkiHat框架测试所需的启动服务。
 *
 * @copyright 2012-2013 SkiHat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    SkiHat Team
 *
 * @package   skihat.kernels
 * @version   1.0
 *
 * @created   2012-08-12
 * @modified  2013-08-31
 */

# 0：关闭调试，1：启用缓存进行调试，2：开启全部调试。
define('SKIHAT_DEBUG', 2);

# 调试缓存开启标志。
define('SKIHAT_DEBUG_CACHE', 1);

define('SKIHAT_CURRENT_DOMAIN', 'www.example.com');

# 文件查询常量。
define('SKIHAT_FILE_EXT', '.inc');
define('SKIHAT_CLASS_SEPARATOR', '#');
define('SKIHAT_PACKAGE_SEPARATOR', '.');


define('SKIHAT_PATH_ROOT', dirname(dirname(dirname(__FILE__))));

define('SKIHAT_PATH_TESTES', dirname(dirname(__FILE__)));
define('SKIHAT_PATH_TESTES_UNITS', dirname(__FILE__));

define('SKIHAT_PATH_APP', SKIHAT_PATH_TESTES_UNITS . '/app');
define('SKIHAT_PATH_APP_MODULES', SKIHAT_PATH_APP . '/modules');
define('SKIHAT_PATH_APP_PUBLICS', SKIHAT_PATH_APP . '/publics');
define('SKIHAT_PATH_APP_PUBLICS_UPLOADS', SKIHAT_PATH_APP_PUBLICS . '/uploads');

define('SKIHAT_PATH_DATA', SKIHAT_PATH_TESTES_UNITS . '/data');
define('SKIHAT_PATH_DATA_CACHES', SKIHAT_PATH_DATA . '/caches');
define('SKIHAT_PATH_DATA_LOGGERS', SKIHAT_PATH_DATA . '/loggers');
define('SKIHAT_PATH_DATA_DOWNLOADS', SKIHAT_PATH_DATA . '/downloads');
define('SKIHAT_PATH_DATA_RUNTIME', SKIHAT_PATH_DATA . '/runtime');

define('SKIHAT_PATH_LIBRARY', SKIHAT_PATH_ROOT . '/skihat');
define('SKIHAT_PATH_VENDOR', SKIHAT_PATH_TESTES_UNITS . '/vendor');

define('SKIHAT_IOC_CLASS', '__class');
define('SKIHAT_IOC_PATHS', '__paths');
define('SKIHAT_IOC_EXT', '__ext');
define('SKIHAT_IOC_INITIAL', '__initial');

define('SKIHAT_MESSAGE_INFO', 'info');
define('SKIHAT_MESSAGE_WARNING', 'warning');
define('SKIHAT_MESSAGE_ERROR', 'error');

define('SKIHAT_MESSAGE_FAIL', 'fail');
define('SKIHAT_MESSAGE_SUCCESS', 'success');


define('SKIHAT_PARAM_MODULE', 'module');
define('SKIHAT_PARAM_PACKAGE', 'package');
define('SKIHAT_PARAM_CONTROLLER', 'controller');
define('SKIHAT_PARAM_ACTION', 'action');
define('SKIHAT_PARAM_FMT', 'fmt');
define('SKIHAT_PARAM_METHOD', '__method__');
define('SKIHAT_PARAM_ID', 'id');

define('SKIHAT_PARAM_REWRITE', 'url');
define('SKIHAT_PARAM_ANCHOR', '#');

define('SKIHAT_PARAM_DEFAULT_MODULE', false);
define('SKIHAT_PARAM_DEFAULT_PACKAGE', false);
define('SKIHAT_PARAM_DEFAULT_CONTROLLER', 'index');
define('SKIHAT_PARAM_DEFAULT_ACTION', 'index');
define('SKIHAT_PARAM_DEFAULT_FMT', 'html');


define('SKIHAT_I18N_LANG', 'zh-CN');
define('SKIHAT_I18N_DEFAULT', 'default');

define('SKIHAT_I18N_DATE_TIME', 'Y-m-d H:i:s');

# 设置当前默认时区为中华人民共和国。
date_default_timezone_set('PRC');

require_once(SKIHAT_PATH_LIBRARY . '/kernels/core.inc');
require_once(SKIHAT_PATH_LIBRARY . '/kernels/skihat.inc');


/**
 * 测试表：
 *
 *  CREATE DATABASE skihat;
 *
 *  use skihat;
 *
 *  CREATE TABLE user (
 *         `id`    INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
 *         `name`  VARCHAR(32) NOT NULL,
 *         `allow` SMALLINT NOT NULL,
 *         `description`   VARCHAR(256) NOT NULL,
 *         `group` INT NULL
 *  );
 *
 *  CREATE TABLE `group` (
 *         `id`  INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
 *         `name` VARCHAR(32) NOT NULL
 *  );
 *
 *  CREATE TABLE `role` (
 *      `id`    INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
 *      `name`  VARCHAR(32) NOT NULL
 *  );
 *
 *  CREATE TABLE `user_role` (
 *      `user_id` INT NOT NULL,
 *      `role_id` INT NOT NULL,
 *      PRIMARY KEY(`user_id`,`role_id`)
 *  );
 *
 *  INSERT INTO `group` (name) VALUES('system');
 *  INSERT INTO `group` (name) VALUES('guest');
 *  INSERT INTO `group` (name) VALUES('emp');
 *
 *  INSERT INTO `role` (name) VALUES('administrators'),('guest');
 */


# 数据库测试引用。
define('MYSQL_CONF','mysql:host=127.0.0.1;dbname=skihat;&user=root&pass=killer&charset=utf8');
define('MYSQL_DSN', 'mysql:host=127.0.0.1;dbname=skihat;');
define('MYSQL_USER', 'root');
define('MYSQL_PASS', 'killer');