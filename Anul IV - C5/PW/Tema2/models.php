<?php

require_once 'idiorm.php';
require_once 'paris.php';

ORM::configure('sqlite:db.sqlite');
Model::$auto_prefix_models = 'Pw';

class PwArticle extends Model {
	public static $_id_column = 'art_id';

    public function author() {
        return $this->has_one('User', 'usr_id', 'art_author');
    }

	public function categories() {
		return $this->has_many_through('Category', 'ArticleCategory', 'artc_art_id', 'artc_cat_id', 'art_id', 'cat_id');
	}
}

class PwCategory extends Model {
	public static $_id_column = 'cat_id';

	public function articles() {
		return $this->has_many_through('Article', 'ArticleCategory', 'artc_cat_id', 'artc_art_id', 'cat_id', 'art_id');
	}
}

class PwArticleCategory extends Model {
}

class PwUser extends Model {
	public static $_id_column = 'usr_id';
}


$tableToModel = array(
	"pw_article" => "Article",
	"pw_article_category" => "ArticleCategory",
	"pw_category" => "Category",
	"pw_user" => "User",
)

?>