<form action="ex4.php" method="POST"> 
    <input name="username" type="text">
    <input name="password" type="text">
    <button type="submit">Login</button> 
</form>

<?php 

	session_start();

	if(isset($_SESSION['user'])) {
		$user = $_SESSION['user'];
		echo $user->username." e autentificat";
		
		echo '<a href="ex6.php"> Logout </a>';
	}
	
?>		
