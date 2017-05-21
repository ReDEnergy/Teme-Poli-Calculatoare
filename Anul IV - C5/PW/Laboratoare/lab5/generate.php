<?php
require_once 'idiorm.php';
ORM::configure('sqlite:db.sqlite');
 
ORM::get_db()->exec('DROP TABLE IF EXISTS articles;');
ORM::get_db()->exec(
        'CREATE TABLE articles (' .
        'id INTEGER PRIMARY KEY AUTOINCREMENT, ' .
        'title TEXT, ' .
        'text TEXT)'
    );
 
ORM::get_db()->exec('DROP TABLE IF EXISTS users;');
ORM::get_db()->exec(
        'CREATE TABLE users (' .
        'id INTEGER PRIMARY KEY AUTOINCREMENT, ' .
        'username VARCHAR(50), ' .
        'password VARCHAR(50), ' .
        'rights VARCHAR(50)' .
        ')'
    );
 
function create_article($title, $contents)
{
    $article = ORM::for_table('articles')->create();
    $article->title = $title;
    $article->text = $contents;
    $article->save();
    return $article;
}
 
function create_user($username, $password, $rights)
{
    $user = ORM::for_table('users')->create();
    $user->username = $username;
    $user->password = $password;
    $user->rights = $rights;
    $user->save();
    return $user;
}
 
 
create_article('Some Article', 'lots of words in article 1');
create_article('Other Article', 'lots of words in article 2');
create_article('Different Article', 'lots of words in article 3');
create_article('Some Other Article', 'lots of words in article 4');
create_article('Some Different Other Article', 'lots of words in article 5');
create_article('Some Other Different Article', 'lots of words in article 6');
create_article('Some Article', 'lots of words in article 7');
create_article('Some Different Article', 'lots of words in article 8');
 
create_user('admin', '21232f297a57a5a743894a0e4a801fc3', '*');
create_user('user', '8287458823facb8ff918dbfabcd22ccb', '1,3,5,7');
create_user('guest', '8287458823facb8ff918dbfabcd22ccb', '1');
create_user('matei', '8287458823facb8ff918dbfabcd22ccb', '2,4,6,8');
 
?>