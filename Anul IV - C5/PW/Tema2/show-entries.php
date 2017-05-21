<?php

	require_once 'models.php';

	// header('Content-Type: text/json');

	if (isset($_GET['table'])) {
		$table = $_GET['table'];
		if (isset($tableToModel[$table])) {
			$values = Model::factory($tableToModel[$table])->find_array();

			echo json_encode($values);
		}
		else
			echo 'wrong_table';
	}
	else
		echo 'wrong_table';
?>