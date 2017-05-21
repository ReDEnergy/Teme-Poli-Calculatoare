<?php
	if (!@$_POST['jsonData']) die();
	
	$recv = json_decode($_POST['jsonData']);
	
	if (!$recv || !$recv->word) die();
	
	echo json_encode(array('definition' => 'This is the tooltip for '. $recv->word));
?>