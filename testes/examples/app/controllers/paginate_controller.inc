<?php

class PaginateController extends ApplicationController {
    public function indexAction() {
        $this['guestbooks'] = GuestBook::fetchAll();
    }

    public function helperAction() {
        $this['guestbooks'] = GuestBook::fetchAll();
    }

    public function actionModels() {
        parent::actionModels();

        Skihat::import('models.guest_book', SKIHAT_PATH_APP);
    }

    public function actionFilters() {
        return array('paginate' => array(
            SKIHAT_IOC_CLASS => 'controllers.filters.paginate_filter',
            SKIHAT_IOC_PATHS => SKIHAT_PATH_LIBRARY,
            'var' => 'guestbooks'
        ));
    }
}