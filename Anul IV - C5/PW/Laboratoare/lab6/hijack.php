<?php
	$url = parse_url('http://localhost:8080/secret.php');
	$host = $url['host'];
	$path = $url['path'];
	
	$ssid = $_COOKIE['PHPSESSID'];
	echo $ssid;
	
	$fp = fsockopen($host, 8080);
	// send the request headers:
	fputs($fp, "GET $path HTTP/1.1\r\n");
	fputs($fp, "Host: $host\r\n");
	fputs($fp, "Connection: close\r\n");
	fputs($fp, "Cookie: PHPSESSID={$ssid};\r\n\r\n");
	
	$result = ''; 
	while(!feof($fp)) {
		// receive the results of the request
		$result .= fgets($fp, 128);
	}
	// close the socket connection:
	fclose($fp);
	
	// split the result header from the content
	$result = explode("\r\n\r\n", $result, 2);
 
	$header = isset($result[0]) ? $result[0] : '';
	$content = isset($result[1]) ? $result[1] : '';

	echo $header;
	echo $content;

?>