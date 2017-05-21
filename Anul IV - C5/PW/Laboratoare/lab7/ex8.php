<?php
	require_once 'idiorm.php';
	require_once '../libs/smarty/Smarty.class.php';
	ORM::configure('sqlite:./db.sqlite');	

	$smarty = new Smarty();

	$persons = ORM::for_table('person')->find_array();
	$page = $_GET['p']; 
	
	$persons = array_slice($persons, $page * 3, 3); 

	$smarty->assign('persons', $persons);
	$smarty->assign('pages', 10);
	$smarty->assign('page', $page);
	$smarty->display('ex8.tpl');
?>