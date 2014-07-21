<?php

class CtlviewController extends ApplicationController {
    public function indexAction() {
        $cat = $this->query('cat', 0);
        $this->render(array('pass' => $cat));
    }

    public function subcatAction() {
        $cat = $this->query('cat', 0);
        $sub = $this->query('sub', 0);

        $this->render(array('pass' => array($cat, $sub)));
    }

    public function templateAction() {
        $this->render(array('template' => 'ctlview.1.2.subcat'));
    }

    public function messageAction() {
        $this->message('显示提示信息', SKIHAT_MESSAGE_SUCCESS);
        $this->redirect('result');
    }

    public function resultAction() {

    }

    public function redirectAction() {
        $this->redirect('index');
        # 这里的代码将会继续执行
    }


    public function textAction() {
        $this->text('Hello Skihat');
    }

    public function jsonAction() {
        $this->json(json_encode(array('name' => 'Hello Skihat')));
    }

    public function jsonpAction() {
        $this->jsonp(json_encode(array('name' => 'Hello Skihat')));
    }

    public function jsonpTextAction() {
        $this->jsonp('Hello Skihat', 'html/text');
    }

    public function errorAction() {
        if ($error = $this->query('errorCode', 404)) {
            $this->error($error);
        }
    }

    public function actionView() {
        return array(
            SKIHAT_IOC_CLASS => self::DEFAULT_VIEW_CLASS,
            'theme' => 'defaults'
        );
    }
}