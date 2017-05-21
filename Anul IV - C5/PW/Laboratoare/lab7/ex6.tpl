<form id="form" method="POST" action="ex6.php">
	<label for="name"> Name </label>
	<input type="text" name="name" value="{$name|escape}" size="40">
	<label for="age"> Age </label>
	<input type="text" name="age" value="{$age|escape}" size="40">
	<input type="submit" value="Submit">
</form>