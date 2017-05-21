<?php
	require_once 'idiorm.php';
	require_once '../libs/smarty/Smarty.class.php';
	ORM::configure('sqlite:./db.sqlite');	

	$smarty = new Smarty();

	$persons = ORM::for_table('expenses')->find_array();
//	var_dump($persons);

	$smarty->assign('persons', $persons);
	$smarty->display('ex3.tpl');

?>