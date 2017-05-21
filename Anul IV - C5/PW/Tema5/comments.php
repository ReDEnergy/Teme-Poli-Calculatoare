<?php
	require_once 'idiorm.php';
	ORM::configure('sqlite:./db.sqlite');
	
	if (isset($_GET['articleID'])) {
		$artID = $_GET['articleID'];

		$comments = ORM::for_table('comment')->where('art_id', $artID)->find_array();
		echo json_encode($comments);
		return;
	}

	function addComment($artID, $name, $email, $message) {
	    $comment = ORM::for_table('comment')->create();
	    $comment->art_id = $artID;
	    $comment->name = $name;
	    $comment->email = $email;
	    $comment->message = $message;
		$comment->date = date("Y-m-d H:i:s");
	    $comment->save();
	    return $comment;
	}

	if (isset($_POST['articleID']) && isset($_POST['name']) && isset($_POST['email']) && isset($_POST['message'])) {
		$artID = $_POST['articleID'];
		$name = $_POST['name'];
		$email = $_POST['email'];
		$message = $_POST['message'];
		
		addComment($artID, $name, $email, $message);
	}


?>
