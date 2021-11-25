<!DOCTYPE html>
<html>
<head>
  <title>Upload your files</title>
</head>
<body>
  <form enctype="multipart/form-data" action="upload.php" method="POST">
    <p>Upload your file</p>
    <input type="file" name="uploaded_file"></input><br />
    <input type="submit" value="Upload"></input>
  </form>
<?PHP
  echo "we do be in this";
  if(!empty($_FILES['uploaded_file']))
  {
    echo "we do be in this2";
    $path = "/mnt/nfs/homes/edal--ce/Desktop/kekw";
    $path = $path . basename( $_FILES['uploaded_file']['name']);

    if(move_uploaded_file($_FILES['uploaded_file']['tmp_name'], $path))
    {
      echo "The file ".  basename( $_FILES['uploaded_file']['name']). 
      " has been uploaded";
    } 
    else
      echo "There was an error uploading the file, please try again!";
  }
  else
    echo "FILES empty";
?>
</body>
</html>
