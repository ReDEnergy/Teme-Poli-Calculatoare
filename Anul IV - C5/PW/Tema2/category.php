<?php

	require_once 'models.php';


	if (isset($_GET['cat_id'])) {
		$cat_id = $_GET['cat_id'];
	}
	else
		DIE('wrong_cat');


	// Get CATEGORY
	$category = Model::factory('Category')->where('cat_id', $cat_id)->find_one();
	if ($category == FALSE)
		DIE('wrong_cat');

	$articles = $category->articles()->order_by_desc('art_publish_date')->find_many();

	function sunteti_idioti($article) {
		$author = $article->author()->find_one();
		$idoti = array();
		$idoti['id'] = $article->art_id;
		$idoti['title'] = $article->art_title;
		$idoti['content'] = $article->art_content;
		$idoti['views'] = $article->art_views;
		$idoti['author'] = $author->usr_username;
		$idoti['publish_date'] = $article->art_publish_date;
		$idoti['update_date'] = $article->art_update_date;

		return $idoti;
	}

	$out = array();

	foreach ($articles as $article) {
		array_push($out, sunteti_idioti($article));
	}

	echo json_encode($out);

?>