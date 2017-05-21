<?php

	require_once 'models.php';

	ORM::get_db()->exec('PRAGMA case_sensitive_like=ON;');

	if (isset($_GET['s'])) {
		$s = $_GET['s'];
		if (strlen($s) == 0)
			DIE('s');
	}
	else
		DIE('s');

	$s = '%'.$s.'%';

	// Get ARTICLE
	$articles = Model::factory('Article')
		->where_raw('(`art_title` LIKE ? OR `art_content` LIKE ?)', array($s, $s))
		->order_by_desc('art_publish_date')
		->find_many();

	if ($articles == FALSE)
		DIE('s');

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