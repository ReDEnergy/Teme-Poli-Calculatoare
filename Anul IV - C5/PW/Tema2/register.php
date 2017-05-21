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

	// Check CONFIRM
	if (isset($_POST['confirm'])) {
		$confirm = $_POST['confirm'];
		if (strcmp($password, $confirm))
			DIE('confirm');
	}
	else
		DIE('confirm');

	// Check PASSWORD STRENGTH
	function checkPassword($pwd, &$errors) {
	    $errors_init = $errors;

	    if (strlen($pwd) < 6) {
	        $errors[] = "Password too short!";
	    }

	    if (!preg_match("#[0-9]+#", $pwd)) {
	        $errors[] = "Password must include at least one number!";
	    }

	    if (!preg_match("#[a-zA-Z]+#", $pwd)) {
	        $errors[] = "Password must include at least one letter!";
	    }

	    return ($errors == $errors_init);
	}

	function generateRandomString($length = 32) {
	    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
	    $randomString = '';
	    for ($i = 0; $i < $length; $i++) {
	        $randomString .= $characters[rand(0, strlen($characters) - 1)];
	    }
	    return $randomString;
	}

	if (checkPassword($password, $errors) == FALSE)
		DIE('password_strength');

	// Check if USER EXISTS
	$exists = Model::factory('User')->where('usr_username', $username)->find_one();
	if ($exists)
		DIE('user_exists');

	// ADD USER EXISTS
	$salt = generateRandomString();
	$user = Model::factory('User')->create();
	$user->usr_username = $username;
	$user->usr_password = sha1($password.$salt);
	$user->usr_salt = $salt;
	$user->usr_register_date = date("Y-m-d H:i:s");
	$user->usr_register_date = date("0000-00-00 00:00:00");
	$user->save();

	echo 'ok';
?>