<?php

	require_once 'models.php';


	if (isset($_GET['art_id'])) {
		$art_id = $_GET['art_id'];
	}
	else
		DIE('wrong_art');


	// Check if USER EXISTS
	$article = Model::factory('Article')->where('art_id', $art_id)->find_one();
	if ($article == FALSE)
		DIE('wrong_art');

	$author = $article->author()->find_one();

	$sunteti_idioti = array();
	$sunteti_idioti['id'] = $article->art_id;
	$sunteti_idioti['title'] = $article->art_title;
	$sunteti_idioti['content'] = $article->art_content;
	$sunteti_idioti['views'] = $article->art_views;
	$sunteti_idioti['author'] = $author->usr_username;
	$sunteti_idioti['publish_date'] = $article->art_publish_date;
	$sunteti_idioti['update_date'] = $article->art_update_date;

	echo json_encode($sunteti_idioti);

	$article->art_views++;
	$article->save();

?>