<?php
/**
 * startup.inc 提供SkiHat框架所需的启动服务。
 *
 * @copyright 2012-2013 SkiHat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    SkiHat Team
 *
 * @package   app.boots
 * @version   1.0
 *
 * @created   2012-08-12
 * @modified  2013-08-31
 */

# 0：关闭调试，1：启用缓存进行调试，2：开启全部调试。
define('SKIHAT_DEBUG',1);
define('SKIHAT_DEBUG_CACHE', 1);

# 文件查询常量。
define('SKIHAT_FILE_EXT', '.inc');
define('SKIHAT_CLASS_SEPARATOR', '#');
define('SKIHAT_PACKAGE_SEPARATOR', '.');

# IOC 控制常量。
define('SKIHAT_IOC_CLASS', '__class');
define('SKIHAT_IOC_PATHS', '__paths');
define('SKIHAT_IOC_EXT', '__ext');
define('SKIHAT_IOC_INITIAL', '__initial');

# 路径常量.
define('SKIHAT_PATH_ROOT', dirname(dirname(dirname(__FILE__))));

# 应用路径常量。
define('SKIHAT_PATH_APP', SKIHAT_PATH_ROOT . '/app');
define('SKIHAT_PATH_APP_MODULES', SKIHAT_PATH_APP . '/modules');
define('SKIHAT_PATH_APP_PUBLICS', SKIHAT_PATH_APP . '/publics');
define('SKIHAT_PATH_APP_PUBLICS_UPLOADS', SKIHAT_PATH_APP_PUBLICS . '/uploads');

# 数据路径常量。
define('SKIHAT_PATH_DATA', SKIHAT_PATH_ROOT . '/data');
define('SKIHAT_PATH_DATA_CACHES', SKIHAT_PATH_DATA . '/caches');
define('SKIHAT_PATH_DATA_LOGGERS', SKIHAT_PATH_DATA . '/loggers');
define('SKIHAT_PATH_DATA_DOWNLOADS', SKIHAT_PATH_DATA . '/downloads');
define('SKIHAT_PATH_DATA_RUNTIME', SKIHAT_PATH_DATA . '/runtime');

# 库常量。
define('SKIHAT_PATH_LIBRARY', dirname(dirname(dirname(dirname(dirname(__FILE__))))) . '/skihat');
define('SKIHAT_PATH_VENDOR', SKIHAT_PATH_ROOT . '/vendor');


# 消息常量。
define('SKIHAT_MESSAGE_INFO', 'info');
define('SKIHAT_MESSAGE_WARNING', 'warning');
define('SKIHAT_MESSAGE_ERROR', 'error');

define('SKIHAT_MESSAGE_FAIL', 'fail');
define('SKIHAT_MESSAGE_SUCCESS', 'success');

# 参数常量。
define('SKIHAT_PARAM_MODULE', 'module');
define('SKIHAT_PARAM_PACKAGE', 'package');
define('SKIHAT_PARAM_CONTROLLER', 'controller');
define('SKIHAT_PARAM_ACTION', 'action');
define('SKIHAT_PARAM_FMT', 'fmt');
define('SKIHAT_PARAM_ID', 'id');

define('SKIHAT_PARAM_REWRITE', 'url');
define('SKIHAT_PARAM_ANCHOR', '#');

# 分页参数。
define('SKIHAT_PARAM_PAGINATE', 'page');
define('SKIHAT_PARAM_PAGINATE_SIZE', 'page_size');
define('SKIHAT_PARAM_PAGINATE_SORT', 'page_sort');
define('SKIHAT_PARAM_PAGINATE_DIR', 'page_dir');

# 方法参数。
define('SKIHAT_PARAM_METHOD', '__method__');

define('SKIHAT_PARAM_DEFAULT_MODULE', false);
define('SKIHAT_PARAM_DEFAULT_PACKAGE', false);
define('SKIHAT_PARAM_DEFAULT_CONTROLLER', 'index');
define('SKIHAT_PARAM_DEFAULT_ACTION', 'index');
define('SKIHAT_PARAM_DEFAULT_FMT', 'html');

# I18N 常量。
define('SKIHAT_I18N_LANG', 'zh-CN');
define('SKIHAT_I18N_DEFAULT', 'default');

define('SKIHAT_I18N_DATE_TIME', 'Y-m-d H:i:s');
define('SKIHAT_I18N_DATE', 'Y-m-d');
define('SKIHAT_I18N_TIME', 'H:i:s');
define('SKIHAT_I18N_WEEK', 'l');


# 声明当前域名常量
define('SKIHAT_CURRENT_DOMAIN', $_SERVER['HTTP_HOST']);


# 引用启动文件。
require_once(SKIHAT_PATH_LIBRARY . '/kernels/core.inc');
require_once(SKIHAT_PATH_LIBRARY . '/kernels/skihat.inc');

# 开启SESSION
session_start();

# 设置错误处理和错误处理函数。
ini_set('log_errors', 'on');
ini_set('error_log', SKIHAT_PATH_DATA_LOGGERS . '/errors.log');


# 设置错误展示方式。
if (SKIHAT_DEBUG) {
    error_reporting(E_ALL);
    ini_set('display_errors', 'on');
} else {
    error_reporting(E_ERROR);
    ini_set('display_errors', 'on');
}


/**
 * 设置当前默认时区为中华人民共和国。
 */
date_default_timezone_set('PRC');


# 注册应用路径。
Skihat::register(array(
        SKIHAT_PATH_APP_MODULES,
        SKIHAT_PATH_APP,
        SKIHAT_PATH_LIBRARY,
        SKIHAT_PATH_VENDOR)
);

# 引用前端控制器。
Skihat::import('appservs.application', array(
        SKIHAT_PATH_APP,
        SKIHAT_PATH_LIBRARY)
);