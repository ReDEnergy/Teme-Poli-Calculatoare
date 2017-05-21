<?php
	$values[0]='red';
	$values[1]='blue';
	
	if (isset($_POST['color'])) {
		$color = $_POST['color'];
		echo array_search($color, $values);
		if (array_search($color, $values) !== false)
			echo "Good ". $color."\n";
		else
			echo "Ha ha - team prins";
	}
	
	
?>
 <form action="ex2.php" method="POST">
	<select name="color">
		<?php
			foreach($values as $value)
				echo "<option value=\"".$value."\">".$value."</option>";
		?>        
	</select>
	<input type="submit" />
</form>