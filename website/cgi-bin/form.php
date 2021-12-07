<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>test</title>
</head>
<body>
  <br>
<!-- Hi <?php echo $_POST['name'];?>. -->


<!-- You are <?php echo (int)$_POST['age']; ?> years old. -->
<br>

<?php
echo "POST :";
  foreach ($_POST as $key => $value) {
        echo "<tr>";
        echo "<td>";
        echo $key;
        echo "</td>";
        echo "<td>";
        echo $value;
        echo "</td>";
        echo "</tr>";
    }
  echo "GET :";
  foreach ($_GET as $key => $value) {
        echo "<tr>";
        echo "<td>";
        echo $key;
        echo "</td>";
        echo "<td>";
        echo $value;
        echo "</td>";
        echo "</tr>";
    }
?>
</body>
</html>
