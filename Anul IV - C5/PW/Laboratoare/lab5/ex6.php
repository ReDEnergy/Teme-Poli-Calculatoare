<?php 
	require_once 'idiorm.php';
	ORM::configure('sqlite:./db.sqlite');
?>

<html>
	<head>
		<meta charset="UTF-8">
		<title> Programare Web </title>
		<link rel="stylesheet" href="style.css" />
	</head>
	<body>

<?php 


	session_start();
	session_destroy();
	header("Location: ex4.php");
?>		
		
	</body>
</html>