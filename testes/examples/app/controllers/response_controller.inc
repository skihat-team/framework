<?php

class ResponseController extends ApplicationController {
    public function cacheAction() {
        $this->response->enableCache(30);
    }

    public function statusAction() {
        $this->autoRender = false;
        $this->response->statusCode(404);
    }

    public function errorAction() {
        $this->error(404);
    }

    public function jsonAction() {
        $this->autoRender = false;
        $this->response->contentType('application/json');
        $this->response->contentBody('[name:skihat]');
    }
}