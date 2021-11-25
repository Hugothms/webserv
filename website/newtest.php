<?php
if (!isset($_SERVER["HTTP_HOST"])) 
{
  parse_str($argv[1], $_GET);
  parse_str($argv[1], $_POST);
}
   if(isset($_FILES['image']))
   {
      echo "IS SET";
      $errors= array();
      $file_name = $_FILES['image']['name'];
      $file_size =$_FILES['image']['size'];
      $file_tmp =$_FILES['image']['tmp_name'];
      $file_type=$_FILES['image']['type'];
      $file_ext=strtolower(end(explode('.',$_FILES['image']['name'])));
      
      $extensions= array("jpeg","jpg","png");
      
      if(in_array($file_ext,$extensions)=== false){
         $errors[]="extension not allowed, please choose a JPEG or PNG file.";
      }
      
      if($file_size > 2097152){
         $errors[]='File size must be excately 2 MB';
      }
      
      if(empty($errors)==true){
         move_uploaded_file($file_tmp,"images/".$file_name);
         echo "Success";
      }
      else
      {
         print_r($errors);
      }
   }
   else
   {
      echo "NO FILE";
   }
?>
<html>
   <body>
      
      <form action="newtest.php" method="POST" enctype="multipart/form-data">
         <input type="file" name="image" />
         <input type="submit"/>
      </form>
      
   </body>
</html>