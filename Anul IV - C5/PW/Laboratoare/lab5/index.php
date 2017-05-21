<?php 
	require_once 'idiorm.php';
	ORM::configure('sqlite:./db.sqlite');
?>

<html>
	<head>
		<meta charset="UTF-8">
		<title> Programare Web </title>
	</head>
	<body>

	<form enctype="multipart/form-data" action="ex3.php" method="POST">
	    <input name="title" type="text">
	    <input name="text" type="text">
	    <input name="image" type="file">
	    <button type="submit">Add article</button> 
	</form>	
		
	</body>
</html>