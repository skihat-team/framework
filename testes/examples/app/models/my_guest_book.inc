<?php

class MyGuestBook extends ApplicationModel {
    public static function config() {
        return array(
            self::META_TABLE => 'guest_book',
            self::META_TABLE_ALIAS => 'GuestBook',
            self::META_DATABASE => 'guest_books'
        );
    }
}