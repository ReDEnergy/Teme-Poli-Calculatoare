$(document).ready(function() {
	
	articleID = $("#article").attr("uid");
	$error = $("#error");
	$error.hide();

	// ADD NEW COMMENT

	function validateEmail(email) { 
		var re = /^(([^<>()[\]\\.,;:\s@\"]+(\.[^<>()[\]\\.,;:\s@\"]+)*)|(\".+\"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/;
		return re.test(email);
	}

	function setError($msg) {
		$error.text($msg);
		$error.show();
	}
	
	$("#submit").click( function() {
		name = $("#name").val();
		email = $("#email").val();
		comment = $("#comment").val();

		// Validate comment
		if (name.length === 0) {
			setError('Add a name');
			return;
		}

		if (validateEmail(email) === false) {
			setError('Invalid email address');
			return;
		}		

		if (comment.length === 0) {
			setError('Add a comment');
			return;
		}

		if (comment.length > 100) {
			setError('Comment should be less than 100 characters');
			return;
		}	
		
		$error.hide();
		
		obj = {
			name : name,
			email : email,
			message : comment,
			articleID : articleID,
			date: "just now"						
		};
		
		// Send AJAX
		$.post("comments.php", obj);

		// Add the comment to the list
		createComment(obj);					
	});

	// GET COMMENTS
	
	$comments = $("#comments");

	function createComment(comment) {
		var $comment = $("<div>", {class: "comment row"});
		var $img = $("<div class='col-sm-2'><div class='image'></div></div>");
		var $info = $("<div>", {class: "col-sm-10"});
		var $row = $("<div>", {class: "row"});
		var $name = $("<div>", {class: "name col-sm-7"});
		var $date = $("<div>", {class: "date col-sm-5"});
		var $row2 = $("<div>", {class: "row"});
		var $email = $("<div>", {class: "email col-sm-12"});
		var $message = $("<div>", {class: "message"});

		$name.text(comment.name);
		$date.text(comment.date);
		$email.text(comment.email);
		$message.text(comment.message);
		$row.append($name);
		$row.append($date);
		$row2.append($email);
		$info.append($row);
		$info.append($row2);
		$info.append($message);
		$comment.append($img);
		$comment.append($info);
		$comments.append($comment);
	};
	
	function insertComents(msg) {
		console.log(msg);
		comments = JSON.parse(msg);
		$comments.text('');
		for(var i in comments)
			createComment(comments[i]);
	};
	
	function getComments() {
		$.ajax({
			url: "comments.php",
			data: {
				articleID: articleID, 
			},
			success: insertComents
		});
	}

	window.setInterval(function() {
		getComments();
	}, 5000);	
	
	getComments();
	
});
