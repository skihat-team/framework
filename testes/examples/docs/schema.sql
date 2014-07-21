/*
 * schema.sql 示例脚本。
 */
CREATE DATABASE guestbooks character set 'UTF8';
USE guestbooks;

CREATE TABLE guest_book (
		`id`			INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
		`user` 	  VARCHAR(32) NOT NULL,
		`email` 	VARCHAR(64) NOT NULL,
		`created`	INT NOT NULL,
		`content`	VARCHAR(256) NOT NULL
);

INSERT INTO guest_book(`user`,`email`,`created`,`content`)
VALUES('skihat','skihat-team@outlook.com',unix_timestamp(),'skihat framework'),
 	    ('php framework','skihat-example@outlook.com',unix_timestamp(),'example framework');