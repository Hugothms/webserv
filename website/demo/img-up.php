<html lang="en">
<body style="background-color: darkslateblue; color: lightgrey;">
<div style="display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100%;">


	<h1>Uploading images using PHP-CGI</h1>


	<form action="img-up.php" method="POST" enctype="multipart/form-data">
         <input type="file" name="image" />
         <input type="submit"/>
      </form>


    <?php
   if(isset($_FILES['image']))
   {
      $errors= array();
      $file_name = $_FILES['image']['name'];
      $file_size =$_FILES['image']['size'];
      $file_tmp = $_FILES['image']['tmp_name'];
      $file_type= $_FILES['image']['type'];
      $tmp = $_FILES['image']['name'];
      $tmp = explode('.',$tmp);
      $file_ext = strtolower(end($tmp));
      
      $extensions= array("jpeg","jpg","png");
      
      if(in_array($file_ext,$extensions)=== false)
      {
         $errors[]="extension not allowed, please choose a JPEG or PNG file.";
      }
      
      if(empty($errors)==true)
      {
         $path = "./uploads/".$file_name ;

         // echo "Path is ".$path ;
         move_uploaded_file($file_tmp,$path);
         echo "Success <br/>";
         echo "<img src=".$path." />";
      }
      else
      {
         print_r($errors);
      }
   }
   else
   {
      echo "No file has been uploaded";
   }
?>



</div>
</body>
</html>