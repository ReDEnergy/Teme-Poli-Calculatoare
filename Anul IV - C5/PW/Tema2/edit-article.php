<?php

	require_once 'models.php';

	function testParam($param) {
		if (isset($_POST[$param])) {
			$value = $_POST[$param];
			if (strlen($value) == 0)
				DIE($param);
			return $value;
		}
		else
			DIE($param);
	}

	// Check ID
	$art_id = testParam('id');

	// Get ARTICLE
	$article = Model::factory('Article')->where('art_id', $art_id)->find_one();
	if ($article == FALSE)
		DIE('id');

	// Check TITLE
	$title = testParam('title');

	// Check CONTENT
	$content = testParam('content');

	// Check AUTHOR
	$author = testParam('author');

	// Check CATEGORY
	if (isset($_POST['cat_id'])) {
		$categories = $_POST['cat_id'];
		if (count($categories) == 0)
			DIE('cat_id');
	}
	else
		DIE('cat_id');

	$test = Model::factory('Category')->where_in('cat_id', $categories)->find_many();
	if (count($test) != count($categories))
		DIE('cat_id');

	// Delete OLD CATEGORIES
	Model::factory('ArticleCategory')->where('artc_art_id', $art_id)->delete_many();

	// Add NEW CATEGORIES
	$category = Model::factory('ArticleCategory');
	foreach ($categories as $cat_id) {
		$category->create();
		$category->artc_art_id = $art_id;
		$category->artc_cat_id = $cat_id;
		$category->save();
	}

	$article->art_title = $title;
	$article->art_content = $content;
	$article->art_author = $author;
	$article->art_update_date = date("Y-m-d H:i:s");
	$article->save();

	echo 'ok'
?>