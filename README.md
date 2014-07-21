Skihat PHP MVC Framework
=========

Skihat 是一个基于 PHP 5.3 + 的高效 MVC 框架,提供安全、日志、事务、缓存、数据 库访问、主题等诸多特性。遵循自然、简洁的设计理念、强调实用性、鼓励模块化开发、遵 循习惯优于配置的编程理念。


Skihat 遵循 Apache 2 开源协议,任何个人或公司都允许在尊重原作者版权的基础上开 发、修改和发布,由 Skihat 开发的企业或个人应用,同时鼓励开发者为 Skihat 提供高质量 的代码和各种扩展组件。

Skihat 的功能和特性
=========
作为一个 PHP MVC 应用框架,Skihat 希望能满足应用的日常开发需求,为此提供相应
的功能和特性。

微核模式设计

Skihat 类是框架的核心类,提供动态配置、调试、优先文件服务、ioc 和 i18n 五个基础
服务,其它服务都是基于这五个核心服务进行构建。

分层设计

自上而下将整个应用分为“应用层”、“应用服务层”和“内核服务层”。为了提高应用
的性能和便于扩展,Skihat 采用白盒模型作为分层的实现方式。  Web MVC
采用常见的单入口 MVC 模式设计,所有请求都由 app/publics/index.php 文件统一处理。

模块化开发

允许将不同的业务划分到不同的模块中,再由模块提供具体的实现,框架从路由、分发、 控制器、视图和模型等多个方面提供支持。
组件化的开发与设计 采用组件化的开发与设计,将不同的功能封装到不同的组件中,由组件提供对外一致性服务。

ActiveRecord

采用标准 ActiveRecord 模式,将数据表封装成模型类,静态方法提供多条记录的操作,
实例方法提供对单条记录的操作(目前仅支持 MySql 数据库)。

视图/主题

采用视图模板的方式提供用户的响应内容,允许设置不同的模板主题,支持布局模板、子视图、助手和表单助手等。

路由器支持

支持普通路由、重写、phpinfo 和 restful 四种路由样式,并提供相应的支持。允许根据
模块、包和数据的不同分别设置不同的子域名。

安全性设计

Skihat 的所有源代码都是由.inc 或.stp 作为文件扩展名,同时源代码默认部署在 Web 根
目录之外,禁止外部直接访问,数据访问采用 PDO 占位符,防止 SQL 注入。

数据读写分离

支持数据库访问的读写分离,允许读写分别由不同的服务器提供。
事务处理 允许将事务由统一的组件提供支持,内部自动完成提交或回滚。
缓存支持 允许在同一应用中使用多种缓存,允许在控制器、视图和子视图的自动缓存支持。


Skihat 的环境需求
=========

作为 PHP MVC 框架,支持任何满足 PHP 环境的 Windows 或 Linux 操作系统,可以运 行于 IIS、Nginx、Apache 在内的多种 Web 服务器,只需要满足以下特定要求:
    PHP5.3+、php_mbstring、json、php_pdo、php_pdo_mysql;
    MySql 5.0 +


获取 Skihat 最新版
=========

为了保证 Skihat 的安全性和完整性,建议通过以下渠道获取源代码:
    从官方网站 http://skihat.cn 下载最新稳定版;
    从 https://github.com/skihat-team/framework 下载最新开发版;

许可协议
=========

Skihat 遵循 Apache 2 开源协议发布 http://www.apache.org/licenses/LICENSE-2.0。
