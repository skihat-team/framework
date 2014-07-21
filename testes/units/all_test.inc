<?php
/**
 * all_test.inc 提供SkiHat框架全部单元测试服务。
 *
 * @copyright 2012-2013 SkiHat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    SkiHat Team
 *
 * @package   skihat
 * @version   1.0
 *
 * @created   2012-08-12
 * @modified  2013-08-31
 */

require_once(dirname(__FILE__) . '/startup.inc');

/**
 * 全部测试，提供架构完整测试服务支持。
 *
 * @package skihat
 */
class AllTest extends PHPUnit_Framework_TestSuite {

    /**
     * @see PHPUnit_Framework_testSukit
     */
    public static function suite() {
        $all = new AllTest('all');

        # 测试内核文件。
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/core_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/iocfa_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/skihat_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/validators_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/collection_base_test.inc');

        # 测试缓存组件。
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/caches/cache_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/caches/engines/file_cache_engine_test.inc');

        # 测试日志组件。
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/loggers/logger_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/loggers/engines/file_logger_engine_test.inc');

        # 富媒体组件。
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/medias/media_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/medias/engines/file_media_engine_test.inc');

        # 消息组件。
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/messages/message_test.inc');

        # 安全组合件。
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/securities/security_test.inc');

        # 事务组件。
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/transactions/transaction_test.inc');

        # 数据库组件
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/databases/database_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/databases/sql_builder_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/kernels/databases/engines/mysql_pdo_engine_test.inc');

        # 应用前端组件
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/applications/application_base_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/applications/application_request_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/applications/application_response_test.inc');

        # 控制器组件
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/controllers/controller_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/controllers/action_filters_test.inc');

        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/models/model_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/models/model_fetch_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/models/model_meta_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/models/model_links_test.inc');

        # 路由组件
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/routers/router_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/routers/router_rules_test.inc');
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/routers/router_styles_test.inc');

        # 视图组件
        $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/views/theme_test.inc');
        # $all->addTestFile(SKIHAT_PATH_TESTES_UNITS . '/skihat/views/__helpers/form_test.inc');

        return $all;
    }
}