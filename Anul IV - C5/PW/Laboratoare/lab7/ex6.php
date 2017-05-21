<?php
	require_once 'idiorm.php';
	require_once '../libs/smarty/Smarty.class.php';
	ORM::configure('sqlite:./db.sqlite');	

	$smarty = new Smarty();
	$smarty->assign('name', 'Andrei');
	$smarty->assign('age', '16');
	$smarty->display('ex6.tpl');
?>