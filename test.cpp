#include <vector>
#include <string>
#include <unistd.h>
#include <iostream>



std::string data = "--------------------------51b54edcb631b748\n\
Content-Disposition: form-data; name=\"name\"\n\
\n\
test\n\
--------------------------51b54edcb631b748\n\
Content-Disposition: form-data; name=\"age\"\n\
\n\
12\n\
--------------------------51b54edcb631b748--";





char *ft_strdup(std::string s)
{
	char *ret = static_cast<char*>(malloc(sizeof(char) * (s.size() + 1)));


	for (int i = 0; i < s.size(); i++)
	{
		ret[i] = s[i];
	}
	ret[s.size()] = 0;
	return ret;
}
int main(int argc, char const *argv[])
{

	std::vector<std::string> ev;
	std::vector<std::string> av;
	int fdout[2];
	int fdin[2];

	

	pipe(fdout);
	pipe(fdin);

// 	std::string body = "--------------------------51b54edcb631b748
// Content-Disposition: form-data; name="name""

	ev.push_back("GATEWAY_INTERFACE=CGI/1.1");
	ev.push_back("SERVER_PROTOCOL=HTTP/1.1");
	ev.push_back("REDIRECT_STATUS=200");
	ev.push_back("HTTP_HOST=cgi-bin/form.php");
	// ev.push_back("HTTP_HOST=cgi-bin/form.php");
	

	ev.push_back("REQUEST_METHOD=POST");
	ev.push_back("SCRIPT_FILENAME=/Users/edal--ce/Desktop/webserv/website/cgi-bin/form.php");
	ev.push_back("SCRIPT_NAME=/website/cgi-bin/form.php");
	ev.push_back("CONTENT_TYPE=multipart/form-data; boundary=------------------------51b54edcb631b748");

	// ev.push_back("QUERY_STRING="+ filepath.substr(filepath.find_first_of('?') + 1));
	ev.push_back("CONTENT_LENGTH=" + std::to_string(data.size()));//+ to_string(headers["Body"].length()) );


	av.push_back("./website/cgi-bin/php-cgi");
	av.push_back("/Users/edal--ce/Desktop/webserv/website/cgi-bin/form.php");


	char **_av = static_cast<char**>(malloc(sizeof(char *) * (av.size() + 1)));
	char **_ev = static_cast<char**>(malloc(sizeof(char *) * (ev.size() + 1)));

	for (size_t j = 0; j < av.size(); j++)
	{
		_av[j] = ft_strdup(av[j]);
	}
	_av[av.size()] = 0;
	for (size_t j = 0; j < ev.size(); j++)
	{
		std::cout << "e"<<j<<":"<<ev[j]<< std::endl;
		_ev[j] = ft_strdup(ev[j]);
	}
	_ev[ev.size()] = 0;



	pid_t pid = fork();
	if (pid == 0)
	{
		dup2(fdin[0], STDIN_FILENO);
		std::cerr << "DOING STUFF\n";
		std::cerr << execve("./website/cgi-bin/php-cgi", _av, _ev) << std::endl;
		std::cerr << "DONE STUFF\n";		
		exit (0);
	}
	else
	{
		write(1,"WR |", 4);
		write(1, data.c_str(), data.size());
		write(1,"|\n", 2);
		write(fdin[1], data.c_str(), data.size());
		close(fdin[1]);
		wait(0);
	}

	// std::cerr << "DOING STUFF\n";
	// std::cerr << execve("./website/cgi-bin/php-cgi", _av, _ev) << std::endl;
	// std::cerr << "DONE STUFF\n";
	return 0;
}