<?php
	$POST = file_get_contents('php://input');
	if (!@$POST) die();
	
	$simpleXMLObj = new SimpleXMLElement($POST);
	
	if (!$simpleXMLObj) die();
	
	header('Content-type: application/xml');
	print '<?xml version="1.0"?><content><definition>This is the tooltip for '. $simpleXMLObj->word .'</definition></content>';
?>