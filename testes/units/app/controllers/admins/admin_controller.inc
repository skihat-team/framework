<?php

class AdminController extends ApplicationController {
    public static $callInvoke = false;

    public function invoke(ApplicationRequest $request, ApplicationResponse $response) {
        self::$callInvoke = true;
    }
}