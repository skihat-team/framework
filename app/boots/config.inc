<?php
/**
 * index_controller.inc 配置文件声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   app.boots
 * @version   1.0
 *
 * @created   2012-08-07
 * @modified  2013-08-12
 */

# 写入缓存配置信息。
Skihat::write('kernels/caches', array(
    'default' => array(
        SKIHAT_IOC_CLASS => 'kernels.caches.engines.file_cache_engine',
        SKIHAT_IOC_PATHS => SKIHAT_PATH_LIBRARY,
        'paths' => SKIHAT_PATH_DATA_CACHES),
    'runtime' => array(
        SKIHAT_IOC_CLASS => 'kernels.caches.engines.file_cache_engine',
        SKIHAT_IOC_PATHS => SKIHAT_PATH_LIBRARY,
        'paths' => SKIHAT_PATH_DATA_RUNTIME,
        'prefix' => 'runtime')
));

# 写入数据库配置信息。
Skihat::write('kernels/databases', array(
    'default' => array(
        SKIHAT_IOC_PATHS => SKIHAT_PATH_LIBRARY,
        SKIHAT_IOC_CLASS => 'kernels.databases.engines.mysql_pdo_engine',
        'conf' => 'mysql:host=127.0.0.1;dbname=test&user=root&pass=killer&charset=utf8'
    )
));

# 写入资源管理配置信息。
Skihat::write('kernels/medias', array(
    'default' => array(
        SKIHAT_IOC_PATHS => SKIHAT_PATH_LIBRARY,
        SKIHAT_IOC_CLASS => 'kernels.medias.engines.file_media_engine'
    )
));

Skihat::write('kernels/securities', array(
    'authentications' => array(),
    'authorizations' => array()
));