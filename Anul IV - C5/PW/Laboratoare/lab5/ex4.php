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

	if (isset($_POST['username']) && $_POST['password']) {
		$user = $_POST['username'];
		$pass = $_POST['password'];	
	
		$pass = md5($pass);
	
		$user = ORM::for_table('users')
			->where('username', $user)
			->where('password', $pass)
			->find_one();
		
		if ($user) {
			echo "Success\n";
			$ssid = $_COOKIE['PHPSESSID'];
			$_SESSION['user'] = $user;
			echo $ssid;
		}
		else {
			echo "Paroala sau user gresit";
		}
	}

?>		
		
	</body>
</html>