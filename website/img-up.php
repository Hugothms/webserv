<?php
   if(isset($_FILES['image']))
   {
      $errors= array();
      $file_name = $_FILES['image']['name'];
      $file_size =$_FILES['image']['size'];
      $file_tmp =$_FILES['image']['tmp_name'];
      $file_type=$_FILES['image']['type'];
      $file_ext=strtolower(end(explode('.',$_FILES['image']['name'])));
      
      $extensions= array("jpeg","jpg","png");
      
      if(in_array($file_ext,$extensions)=== false)
      {
         $errors[]="extension not allowed, please choose a JPEG or PNG file.";
      }
      
      // if($file_size > 2097152){
      //    $errors[]='File size must be excately 2 MB';
      // }  
      if(empty($errors)==true)
      {
         $path = "./cgi-bin/UPLOADS/".$file_name ;

         echo "Path is ".$path ;
         move_uploaded_file($file_tmp,$path);
         echo "Success <br/>";
         echo "<img src=".$path." height=200 width=300 />";
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
      
      <form action="img-up.php" method="POST" enctype="multipart/form-data">
         <input type="file" name="image" />
         <input type="submit"/>
      </form>
      
   </body>
</html>