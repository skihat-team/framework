<?php

class CallFilter implements IActionFilter {
    function initialize(Controller $controller) {
        echo 'initialize;';
    }

    function invoke($actionMethod) {
        echo 'invoke:', $actionMethod,';';
        return true;
    }

    function render(array &$options) {
        echo 'render;';
        return true;
    }

    function complete() {
        echo 'complete;';
    }
}