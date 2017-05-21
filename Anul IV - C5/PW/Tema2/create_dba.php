<?php
require_once 'idiorm.php';

ORM::configure('sqlite:db.sqlite');

ORM::get_db()->exec('DROP TABLE IF EXISTS pw_article;');
ORM::get_db()->exec(
	'CREATE TABLE pw_article ('.
	'art_id INTEGER PRIMARY KEY AUTOINCREMENT, '.
	'art_title TEXT, '.
	'art_content TEXT, '.
	'art_views INTEGER, '.
	'art_publish_date DATETIME, '.
	'art_update_date DATETIME, '.
	'art_author INTEGER, '.
 	'FOREIGN KEY(art_author) REFERENCES pw_user(usr_id)'.
	')'
);

ORM::get_db()->exec('DROP TABLE IF EXISTS pw_category;');
ORM::get_db()->exec(
	'CREATE TABLE pw_category ('.
	'cat_id INTEGER PRIMARY KEY AUTOINCREMENT, '.
	'cat_title VARCHAR(254)'.
	')'
);


ORM::get_db()->exec('DROP TABLE IF EXISTS pw_article_category;');
ORM::get_db()->exec(
	'CREATE TABLE pw_article_category (' .
	'artc_art_id INTEGER, ' .
	'artc_cat_id INTEGER, ' .
 	'FOREIGN KEY(artc_art_id) REFERENCES pw_article(art_id)'.
 	'FOREIGN KEY(artc_cat_id) REFERENCES pw_category(cat_id)'.
	')'
);

ORM::get_db()->exec('DROP TABLE IF EXISTS pw_user;');
ORM::get_db()->exec(
	'CREATE TABLE pw_user ('.
	'usr_id INTEGER PRIMARY KEY AUTOINCREMENT, '.
	'usr_username VARCHAR(256), '.
	'usr_password CHAR(40), '.
	'usr_salt CHAR(40), '.
	'usr_register_date DATETIME, '.
	'usr_last_login DATETIME '.
	')'
);

require_once 'add_data.php';

?>