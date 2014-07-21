<?php

class CacheController extends ApplicationController {
    public function indexAction() {
        $this->text('Time:' . date(SKIHAT_I18N_DATE_TIME));
    }

    public function actionFilters() {
        return array(
            'cache' => array(
                SKIHAT_IOC_CLASS => 'controllers.filters.cache_filter',
                SKIHAT_IOC_PATHS => SKIHAT_PATH_LIBRARY,
                'actionMethods' => '*'
            )
        );
    }
}