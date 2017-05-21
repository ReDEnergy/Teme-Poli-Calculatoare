<?php

require_once 'models.php';

function loadData($file, $model) {

	$dba_loc = 'database/';
	$lines = file($dba_loc.$file);
	$columns = explode(',', array_shift($lines));

	for ($i = 0; $i < count($columns); $i++) {
		$columns[$i] = trim($columns[$i]);
	}

	$model  = Model::factory($model);

	foreach ($lines as $line) {
		$line = explode(',', $line);

		for ($i = 0; $i < count($line); $i++) {
			$row[$columns[$i]] = trim($line[$i], "\"\n");
		}

		$model->create($row);
		$model->save();
	}
}

loadData('pw_user.csv', 'User');
loadData('pw_article.csv', 'Article');
loadData('pw_category.csv', 'Category');
loadData('pw_article_category.csv', 'ArticleCategory');

?>