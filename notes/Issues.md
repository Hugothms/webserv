Issues:

- Lorsque qu'on soumet une 2eme requete http apres une 1ere, le buffer du serveur n'est pas reinitialise:
Etapes pour reproduire le pb:
	curl -X POST -H "Header de hugo: ca devrai pas etre la" localhost:8080
puis
	curl localhost:8080
le "header de hugo" est present dans la 2eme requete alors qu'il ne devrai pas y etre

