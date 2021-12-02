<?php
//$uploaddir = '/Users/theophile/42/webserv/git_webserv/test_pages/test_upload/upload_dir/';
$uploaddir = '/home/user42/webserv/git_webserv/test_pages/test_upload/upload_dir/';
$uploadfile = $uploaddir . basename($_FILES['userfile']['name']);

echo '<pre>';
print_r($_SERVER);
echo "uploadfile: ".$uploadfile."\n";
print_r($_POST);
if (move_uploaded_file($_FILES['userfile']['tmp_name'], $uploadfile)) {
    echo "Le fichier est valide, et a été téléchargé
           avec succès. Voici plus d'informations :\n";
} else {
    echo "Attaque potentielle par téléchargement de fichiers.
          Voici plus d'informations :\n";
}

echo 'Voici quelques informations de débogage :';
print_r($_FILES);

echo '</pre>';
?>