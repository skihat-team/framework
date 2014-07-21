<?php

class GuestbooksController extends ApplicationController {
    public function indexAction() {
        # 将数据的响应结果设置为视图变量
        $this['guestbooks'] = GuestBook::fetchAll();
    }

    public function writeAction() {
        if ($this->isPost()) {
            $guestbook = new Guestbook($this->form('guestbook'));
            if ($guestbook->save()) {
                $this->text('发布留言成功');
            } else {
                $this->text('发布留言失败，请检查发布内容');
            }
        }
    }

    public function newAction() {
        $this->render('write');
    }

    # 重载父类方法，加载当前模型所需要模型
    public function actionModels() {
        # 使用import方法加载模型实例。
        Skihat::import('models.guest_book', SKIHAT_PATH_APP);
    }
}