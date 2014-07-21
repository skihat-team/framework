<?php

Skihat::import('controllers.controller',SKIHAT_PATH_LIBRARY);

class ApplicationController extends Controller {
    public function actionView() {
        return array(
            SKIHAT_IOC_CLASS => self::DEFAULT_VIEW_CLASS,
            'theme' => 'site'
        );
    }
}