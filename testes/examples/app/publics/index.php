<?php

# 导入启动核心文件。
require(dirname(dirname(__FILE__)) . '/boots/startup.inc');

# 开启前端控制器，并初始化和分发控制器。
$app = new Application();
$app->initialize();
$app->dispatching();