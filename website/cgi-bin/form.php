Hi <?php echo $_POST['name']; 

if (!isset($_SERVER["HTTP_HOST"])) 
{
  parse_str($argv[1], $_GET);
  parse_str($argv[1], $_POST);
}
?>.

<?php
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
?>
You are <?php echo (int)$_POST['age']; ?> years old.