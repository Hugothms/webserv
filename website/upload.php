<html>
<head>
<title>PHP File Upload example</title>
</head>
<body>

<form action="upload.php" enctype="multipart/form-data" method="post">
Select image :
<input type="file" name="file"><br/>
<input type="submit" value="Upload" name="Submit1"> <br/>


</form>
<?php
if(isset($_POST['Submit1']))
{ 
  $filepath = "/tmp/" . $_FILES["file"]["name"];
  echo "filepaht is :".$filepath;

if(move_uploaded_file($_FILES["file"]["name"], $filepath)) 
{
  echo "<img src=".$filepath." height=200 width=300 />";
} 
else 
{
  echo "Error !!";
}
} 
?>

</body>
</html>