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
		
	<pre>	
<?php 


	$person = ORM::for_table('person')->find_one();

	$persons = ORM::for_table('person')->find_many();
	foreach ($persons as $person) {
		echo $person->name.' '.$person->age."\n";
	};
	
	echo "\n\n";
	
	$persons = ORM::for_table('person')->where_like('name', '%lia')->find_many(); 
	
	foreach ($persons as $person) {
		echo $person->name.' '.$person->age."\n";
	};	

	echo "\n\n";
	
	$persons = ORM::for_table('person')->where_like('name', '%an')->find_many(); 
	
	foreach ($persons as $person) {
		echo $person->name.' '.$person->age."\n";
	};	

	echo "\n\n";
	
	function getPersonMessages($person_id) {
		$msgs = ORM::for_table('messages')
			->where('person_fk', $person_id)
			->find_many();
			
		$cnt = count($msgs);
		if ($cnt) {
			
			echo "\t".$cnt." messages \n";	
			foreach ($msgs as $msg)
				echo "\t".$msg->message;
			echo "\n";
		};
	};

	$persons = ORM::for_table('person')->find_many();
	
	foreach ($persons as $person) {
		echo $person->id.". ".$person->name.":\n";
		getPersonMessages($person->id);
	};	

	echo "Exercitiul 7\n\n";
	$msgs = ORM::for_table('messages')
		->join('person', array('person.id', '=', 'messages.person_fk'))
		->where_lte('person.age', 40)
		->find_many();

	foreach ($msgs as $msg)
		echo "\t".$msg->message;
	
	echo "Exercitiul 8\n\n";	

	echo "\n\n";
	
	function getFriends($person_id) {
		$friends = ORM::for_table('friends')
			->where('person_A', $person_id)
			->find_many();
			
		$cnt = count($friends);
		if ($cnt) {
			
			echo "\t".$cnt." friends \n";	
			foreach ($friends as $frnd)
				echo "\t".$frnd->person_B;
			echo "\n";
		};
	};

	foreach ($persons as $person) {
		echo $person->id.". ".$person->name.":\n";
		getFriends($person->id);
	};
	
	$friends = ORM::for_table('friends')
		->join('person', array('person.id', '=', 'friends.person_A'))
		->join('person', array('person.id', '=', 'friends.person_B'))
		->find_many();

	foreach ($friends as $friend) {
		echo $person->name.". ".$person->name. " ".$person->name.":\n";
	}
	

?>		
	</pre>	
		
	</body>
</html>