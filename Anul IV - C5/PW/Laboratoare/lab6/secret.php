<?php

	session_start();	

	if(isset($_SESSION['user'])) {
		$user = $_SESSION['user'];
		echo $user." e autentificat";
	}
	else {
		echo "Nu esti autentificat!";
	}

?>