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

	require('include.php');

	if (isset($user)) {
		echo $user->rights;
		
		$articles = ORM::for_table('articles')->find_many();

		if ($user->rights == '*') {
			foreach ($articles as $article)
				echo '<div><a href="ex1.php?id='.$article->id.'">'.$article->text.'</a></div>';
		}
		else {
			$rights = explode(',', $user->rights);
			foreach ($articles as $article) {
				foreach ($rights as $right) {
					if ($article->id == $right)
						echo '<div><a href="ex1.php?id='.$article->id.'">'.$article->text.'</a></div>';
				}
			}
		}
				
		$rights = explode(',', $user->rights);
		
	}


?>		
		
	</body>
</html>