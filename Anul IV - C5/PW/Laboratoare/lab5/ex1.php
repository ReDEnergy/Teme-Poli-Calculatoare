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

	<div>		
<?php 

	require('include.php');

	$id = $_GET['id'];
	echo $id."\n";
	$article = ORM::for_table('articles')
		->where('id', $id)
		->find_one();	

	echo $article->text;

?>		
	</div>
	<?php echo '<img src="images/'.$article->id.'.png" />' ?>

	<?php
	
		if (isset($user)) {
			$articles = '';
			if (isset($_SESSION['articles']))
				$articles = $_SESSION['articles'];
			$articles = explode(',', $articles);
			array_push($articles, $id);
			$articles = implode(',', $articles);
			$_SESSION['articles'] = $articles;
			echo "Articles".$articles;
		}
		
		
	echo session_save_path();	
	?>


	</body>
</html>