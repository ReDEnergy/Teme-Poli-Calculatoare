<?php

	require_once 'models.php';

	function testParam($param) {
		if (isset($_POST[$param])) {
			$value = $_POST[$param];
			if (strlen($value) < 6)
				DIE($param);
			return $value;
		}
		else
			DIE($param);
	}

	// Check USERNAME
	$username = testParam('username');

	// Check PASSWORD
	$password = testParam('password');

	// Check if USER EXISTS
	$user = Model::factory('User')->where('usr_username', $username)->find_one();
	if ($user == FALSE)
		DIE('user_doesnt_exist');

	if (strcmp($user->usr_password, sha1($password.$user->usr_salt)))
		DIE('wrong_password');

	$user->usr_last_login = date("Y-m-d H:i:s");
	$user->save();

	echo 'ok';
?>