<?php
    class IndexController extends ApplicationController {
        public function indexAction() {
            $this['hello'] = 'Hello Skihat';
        }
    }