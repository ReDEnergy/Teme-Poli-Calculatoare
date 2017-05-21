<?php
require_once 'idiorm.php';
ORM::configure('sqlite:db.sqlite');

ORM::get_db()->exec('DROP TABLE IF EXISTS article;');

ORM::get_db()->exec(
	'CREATE TABLE article ('.
	'id INTEGER PRIMARY KEY AUTOINCREMENT, '.
	'title VARCHAR(150), '.
	'subtitle VARCHAR(150), '.
	'content TEXT '.
	')'
);

ORM::get_db()->exec('DROP TABLE IF EXISTS comment;');

ORM::get_db()->exec(
	'CREATE TABLE comment ('.
	'id INTEGER PRIMARY KEY AUTOINCREMENT, '.
	'art_id INTEGER, '.
	'name VARCHAR(50), '.
	'email VARCHAR(50), '.
	'date DATETIME, '.
	'message TEXT, '.
 	'FOREIGN KEY(art_id) REFERENCES article(id)'.
	')'
);


function addComment($article) {
    $comment = ORM::for_table('article')->create();
    $comment->id = $article[0];
    $comment->title = $article[1];
    $comment->subtitle = $article[2];
    $comment->content = $article[3];
    $comment->save();
    return $comment;
}

$lines = file("article.csv");

foreach ($lines as $line) {
	$line = explode('"|"', $line);
	addComment($line);
}

?>