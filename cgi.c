int main(int argc, char const *argv[])
{
	char *test = strdup("/Users/edal--ce/Desktop/webserv/test.php");
	char **tab = malloc(sizeof(char*) * 3);
	tab[0] = test;
	tab[1] = test;
	tab[2] = 0;
	execve("/usr/bin/php",tab ,0);
	free(test);
	free(tab);
	return 0;
}