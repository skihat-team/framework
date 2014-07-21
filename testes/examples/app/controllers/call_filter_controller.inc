<?php

class CallFilterController extends ApplicationController {

    public function indexAction() {
        $this->autoRender = false;
        echo 'indexAction;';
    }

    public function actionFilters() {
        return array(
            'call' => array(
                SKIHAT_IOC_CLASS => 'controllers.filters.call_filter',
                SKIHAT_IOC_PATHS => SKIHAT_PATH_APP
            )
        );
    }
}