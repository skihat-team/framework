<?php
/**
 * index_controller.inc 默认控制器声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   app.controllers
 * @version   1.0
 *
 * @created   2012-08-07
 * @modified  2013-08-12
 */

Skihat::write('kernels/caches', array(
    'default' => array(
        SKIHAT_IOC_CLASS => 'kernels.caches.engines.file_cache_engine',
        SKIHAT_IOC_PATHS => SKIHAT_PATH_LIBRARY,
        'paths' => SKIHAT_PATH_DATA_CACHES),
    'runtime' => array(
        SKIHAT_IOC_CLASS => 'kernels.caches.engines.file_cache_engine',
        SKIHAT_IOC_PATHS => SKIHAT_PATH_LIBRARY,
        'paths' => SKIHAT_PATH_DATA_RUNTIME)
));


Skihat::write('kernels/databases',array(
    'default' => array(
        SKIHAT_IOC_CLASS => 'kernels.databases.engines.mysql_pdo_engine',
        'conf' => 'mysql:host=127.0.0.1;dbname=guestbooks&user=root&pass=killer&charset=utf8'
    ),
    'guest_books' => array(
        SKIHAT_IOC_CLASS => 'kernels.databases.engines.mysql_pdo_engine',
        'conf' => 'mysql:host=127.0.0.1;dbname=guestbooks&user=root&pass=killer&charset=utf8'
    ),
));
