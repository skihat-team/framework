<?php
/**
 * routing.inc 提供MVC路由配置声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.applications
 * @version   1.0
 *
 * @created   2012-07-22
 * @modified  2013-08-28
 */

/*
 # 默认路由设置
Router::domain(SKIHAT_CURRENT_DOMAIN)
    ->rule('#NormalRouterRule');
*/

Router::domain('www.example.com')
    ->rule(array(SKIHAT_IOC_CLASS => '#NormalRouterRule',
        SKIHAT_PARAM_PACKAGE => array(false,'/^admins/')));  # 设置包参数为非admins值，禁止直接访问。


Router::domain('admins.example.com')
    ->style('#NormalRouterRule')
    ->rule(array(SKIHAT_IOC_CLASS => '#NormalRouterRule',
        SKIHAT_PARAM_PACKAGE => array('admins','admins',true))); # 设置包必须为admins，禁止其它访问。

