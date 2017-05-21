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

	function addArtcile($title, $text)
	{
	    $article = ORM::for_table('articles')->create();
	    $article->title = $title;
	    $article->text = $text;
	    $article->save();
	    return $article;
	}
	
	$title = $_POST['title'];
	$text = $_POST['text'];

	$article = addArtcile($title, $text);

	$info = pathinfo($_FILES['image']['name']);
	$ext = $info['extension']; // get the extension of the file
	$newname = $article->id.'.'.$ext; 

	echo $article->id;

	$target = 'images/'.$newname;
	move_uploaded_file( $_FILES['image']['tmp_name'], $target);



?>		
		
	</body>
</html>