<?php
	$http_response = '';
	
	$fp = fsockopen('localhost', 8080);
	
	fputs($fp, "POST /script1.php HTTP/1.1\r\n");
	fputs($fp, "Host: localhost\r\n");
	fputs($fp, "Content-type: application/x-www-form-urlencoded\r\n");
	fputs($fp, "Content-length: 23\r\n\r\nkey1=value1&key2=value2");
	
		
	while (!feof($fp))
	{
	    $http_response .= fgets($fp, 128);
	}
	fclose($fp);
	
	echo nl2br(htmlentities($http_response));
?>HTTP/1.1\r\ne