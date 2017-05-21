<!DOCTYPE HTML>

<html>

	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
		<title>Articol</title>
		<link rel="stylesheet" href="css/bootstrap.min.css"/>
		<link rel="stylesheet" href="css/style.css"/>
		<script type="text/javascript" src="js/jquery-1.11.1.min.js"></script>
		<script type="text/javascript" src="js/bootstrap.min.js"></script>
		<script type="text/javascript" src="js/script.js"></script>
	</head>
	
	<?php
		require_once 'idiorm.php';
		ORM::configure('sqlite:db.sqlite');
			
		$artID = 0;
		if (isset($_GET['article'])) {
			$artID = $_GET['article'];
		}		
		$article = ORM::for_table('article')->where('id', $artID)->find_one();
		if ($article == false) 
			$article = ORM::for_table('article')->find_one();
	?>
	
	<body>

		<div id="pages">
			<a href="?article=1"> Articol 1 </a>
			<a href="?article=2"> Articol 2 </a>
			<a href="?article=3"> Articol 3 </a>
		</div>		
		
		<article id="article" uid="<?php echo $article->id; ?>">
			<section>
				<header>
					<h1> <?php echo $article->title; ?></h1>
				</header>
					<h4> <?php echo $article->subtitle; ?></h4>
					<?php echo $article->content; ?>
			</section>
			<section>
				<hr />
				<h3> Comments </h3>
				<hr />
				<div id="comments"></div>
				<form class="form-horizontal" role="form">
					<hr />
					<div id="error" class="alert alert-warning"> </div>
					<div class="form-group">
						<label for="name" class="col-sm-2 control-label">Name</label>
						<div class="col-sm-10">
							<input type="text" class="form-control" id="name" placeholder="Name">
						</div>
					</div>
					<div class="form-group">
						<label for="email" class="col-sm-2 control-label">Email</label>
						<div class="col-sm-10">
							<input type="text" class="form-control" id="email" placeholder="Email">
						</div>
					</div>
					<div class="form-group">
						<label for="comment" class="col-sm-2 control-label">Comment</label>
						<div class="col-sm-10">
							<textarea id="comment" class="form-control" rows="2"></textarea>
						</div>
					</div>
					<div class="form-group">
						<div class="col-sm-12">
							<button id="submit" type="button" class="btn btn-primary">Add comment</button>
						</div>
					</div>
				</form>
			</section>
		</article>
	<body>
</html>