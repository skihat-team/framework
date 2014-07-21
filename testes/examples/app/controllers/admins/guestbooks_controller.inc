<?php

class GuestbooksController extends ApplicationController {
    public function indexAction() {
        $this['guestbooks'] = Guestbook::fetchAll();
    }

    public function deleteAction() {
        if ($id = $this->query('id')) {
            if (Guestbook::deleteAll(intval($id))) {
                $this->text('删除留言成功');
            } else {
                $this->text('删除留言失败');
            }
        }
    }


    public function actionModels() {
        parent::actionModels();
        Skihat::import('models.guest_book', SKIHAT_PATH_APP);
    }

}