<html lang="en">
<body style="background-color: darkslateblue; color: lightgrey;">
<div style="display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100%;">
	<h1>Passing form data to PHP-CGI</h1>


<?php 

	if(isset($_GET["name"]))
	{
		echo "Using the GET method to pass data<br/>";
		echo "Your name is ".$_GET["name"].".<br/>";
		echo "You are ".$_GET["age"]." years old.<br/>";
	}
	else if(isset($_POST["name"]))
	{
		echo "Using the POST method to pass data<br/>";
		echo "Your name is ".$_POST["name"].".<br/>";
		echo "You are ".$_POST["age"]." years old.<br/>";
	}
	else
	{
		echo "Requiered data hasn't been provided<br/>";
	}
?>

	<h2>Post mode</h2>
<form action="/form.php" method="post">
 	<p>Your name: <input type="text" name="name" /></p>
 	<p>Your age: <input type="text" name="age" /></p>
 	<p><input type="submit" /></p>
</form>

 	<h2>Get mode</h2>
 <form action="/form.php" method="get">
 	<p>Your name: <input type="text" name="name" /></p>
 	<p>Your age: <input type="text" name="age" /></p>
 	<p><input type="submit" /></p>
 </form>




</div>
</body>
</html>
