<?php
	
	require_once '../libs/smarty/Smarty.class.php';

	$smarty = new Smarty();
	$smarty->debugging = true;
	$smarty->assign('test', 'Hello World');
	$smarty->display('ex2.tpl');

?>