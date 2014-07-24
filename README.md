# INTRODUCTION

Skihat is a MVC framework based on PHP 5.3+. It brings the features of security, log, trasaction, buffer/cache, database access, themes, etc.

<<<<<<< HEAD
Skihat 是一个基于 PHP 5.3 + 的高效 MVC 框架,提供安全、日志、事务、缓存、数据 库访问、主题等诸多特性。遵循自然、简洁的设计理念、强调实用性、鼓励模块化开发、遵 循约定优于配置的编程理念。
=======
We use the **convention over configuration** ( CoC ) pattern, to make it as simple as it is powerful.
>>>>>>> 973c62f03bd47fe290ccf07f66645e8fd3118a5a

## FEATURES

### Micro Kernel
Skihat as the core object. It provides:

- Dynamic Configuration
- Priority Document Service
- IOC
- i18n

### Hierarchical

- Application Layer
- Service Layer
- Kernel Layer
- White-box Model
- Single Entry, all requests are processed by app/publics/index.php

### Modular
- Routing
- Deploying
- Model
- Controller
- View

### Active Record
- Active Record Pattern
- Table as Model
- Static methods support multi-record operation. Instance method supports single record operation.

### View
- Theming
- Layout Template
- Sub-view
- Assistant & Menu Assistant

### URL Routing

- Supports: static, rewrite, phpinfo, restful
- Per-module configuration

### Security

- Configuration file uses .inc or .stp as extention name
- Source code deploy out of web root by default
- Use PHP Data Objects (PDO) extension

### Database Access
- Read-Write Splitting
- Transaction
- Caching

## REQUIREMENTS

- PHP 5.3+

- php_mbstring

- json

- php_pdo

- php_pdo_mysql

- MySQL 5.0+

- Apache/Nginx/IIS

## INSTALLATION

- Stable Version: http://skihat.cn/#downloads

- Development Version: https://github.com/skihat-team/framework

# LICENSE
Apache 2.0
