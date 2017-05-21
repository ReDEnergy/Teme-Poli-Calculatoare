<?php

	session_start();

	if (isset($_POST['ceva'])) {
		$_SESSION['ceva'] = $_POST['ceva'];
		echo $_SESSION['ceva'];
	}

	if (isset($_GET['ceva'])) {
		echo $_SESSION['ceva'];
	}
	
?>