<?php

class RequestController extends ApplicationController {
    public function indexAction() {
        $name = $this->request->query('name');
        $this->text($name);
    }

    public function templateAction() {

    }

    public function queryAction() {
        $this->text($this->request->query('name'));
    }

    public function queryAllAction() {
        $this->autoRender = false;
        print_r($this->request->queryAll());
    }

    public function formAction() {
        if ($this->isPost()) {
            $this->text($this->request->form('name'));
        }
    }

    public function fileAction() {
        if ($this->isPost()) {
            print_r($this->file('file'));
        }
    }

    public function multipleAction() {
        if ($this->isPost()) {
            print_r($this->file('file'));
        }
    }
}