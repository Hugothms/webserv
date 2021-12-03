/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 17:21:43 by hthomas           #+#    #+#             */
/*   Updated: 2021/12/03 18:05:01 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

map<unsigned int, string> create_map_return_codes(void)
{
	map<unsigned int, string> codes;
	codes[100] = "100 Continue";
	codes[101] = "101 Switching Protocols";
	codes[200] = "200 OK";
	codes[201] = "201 Created";
	codes[202] = "202 Accepted";
	codes[203] = "203 Non-Authoritative Information";
	codes[204] = "204 No Content";
	codes[205] = "205 Reset Content";
	codes[300] = "300 Multiple Choices";
	codes[301] = "301 Moved Permanently";
	codes[302] = "302 Found";
	codes[303] = "303 See Other";
	codes[305] = "305 Use Proxy";
	codes[306] = "306 (Unused)";
	codes[307] = "307 Temporary Redirect";
	codes[400] = "400 Bad Request";
	codes[402] = "402 Payment Required";
	codes[403] = "403 Forbidden";
	codes[404] = "404 Not Found";
	codes[405] = "405 Method Not allow";
	codes[406] = "406 Not Acceptable";
	codes[408] = "408 Request Timeout";
	codes[409] = "409 Conflict";
	codes[410] = "410 Gone";
	codes[411] = "411 Length Required";
	codes[413] = "413 Payload Too Large";
	codes[414] = "414 URI Too Long";
	codes[415] = "415 Unsupported Media Type";
	codes[417] = "417 Expectation Failed";
	codes[426] = "426 Upgrade Required";
	codes[500] = "500 Internal Server Error";
	codes[501] = "501 Not Implemented";
	codes[502] = "502 Bad Gateway";
	codes[503] = "503 Service Unavailable";
	codes[504] = "504 Gateway Timeout";
	codes[505] = "505 HTTP Version Not Supported";
	return codes;
}

map<unsigned int, string> codes = create_map_return_codes();

Request::~Request()
{
	if (this->location)
		delete(this->location);
	// free(static_cast<void *>(this->location));
}

Request::Request(const string &buffer)
:  code(0), passed_cgi(false)
{
	size_t pos = 0;

	DEBUG(endl << endl << "******* NEW REQUEST: ********");
	DEBUG(buffer);
	DEBUG("END");

	type = get_str_before_char(buffer, " ", &pos);
	target = get_str_before_char(buffer, " ", &pos);
	get_str_before_char(buffer, "\n", &pos);

	size_t len = buffer.length();
	while (pos <= len && buffer[pos])
	{
		if (buffer[pos] == '\n')
			pos++;
		string header = get_str_before_char(buffer, ":\n", &pos);
		if (header.length() == 0)
			break ; // case empty line
		if (header == "Host")
		{
			string ip_address;
			string port;
			if ((ip_address = get_str_before_char(buffer, ":", &pos)).length())
				port = get_str_before_char(buffer, "\r\n", &pos);
			else
			{
				ip_address = get_str_before_char(buffer, "\r\n", &pos);
				port = "80";
			}
			headers.insert(pair<string, string>("Host", ip_address));
			headers.insert(pair<string, string>("Port", port));
			continue;
		}
		// DEBUG("INSERTING");
		headers.insert(pair<string, string>(header, get_str_before_char(buffer, "\r\n", &pos)));
	}
	// DEBUG("HEADERS BE LIKE " << headers.count("Content-Length"))
	if (headers.count("Content-Length") > 0)
	{
		// DEBUG("WE ARE INSERTING");
		headers.insert(pair<string, string>("Body", &buffer[pos]));
	}
	// for (map<string, string>::iterator it = headers.begin(); it != headers.end(); it++)
	// 	DEBUG(it->first << ": " << it->second);
	// DEBUG("****** REQUEST PARSED *******");
}

string to_string_custom(const int &error_code)
{
	stringstream ret;
	ret << error_code;
	return ret.str();
}

string	Request::error_page(const int error_code)
{
	if (server && server->get_error_pages().size() && server->get_error_pages()[error_code].length())
			return (server->get_root() + server->get_error_pages()[error_code]);
	return ("default_error_pages/" + to_string_custom(error_code) + ".html");
}

/**
 * @param servers	list of server in which to search for the corresponding one
 * @param host		specified in the conf by the 'server_name' keyword
 * @param port		specified in the conf by the 'listen' keyword (ip_address:port)
 * @return			a pointer to the server corresponding to the couple (host, port) or the default server for this port if it exist, null overwise.
**/
bool	Request::select_server(const list<Server*> &servers)
{
	string host = headers["Host"];
	unsigned int port = atoi(headers["Port"].c_str());
	this->server = NULL;
	for (list<Server*>::const_iterator server = servers.begin(); server != servers.end(); server++)
	{
		if ((*server)->get_port() == port)
		{
			if (this->server == NULL)
				this->server = *server;
			list<string> server_names = (*server)->get_server_names();
			for (list<string>::iterator server_name = server_names.begin(); server_name != server_names.end(); server_name++)
			{
				if ((*server_name == "0.0.0.0" || *server_name == host) && (*server)->get_port() == port)
				{
					this->server = *server;
					return true;
				}
			}
		}
	}
	if (!this->server)
		return false;
	DEBUG("Found default server for port: " << port);
	return true;
}

// string get_bin(char *path)
// {
// 	string ex(path);


// 	if (ex.find(".php") != string::npos)
// 		return ("./website/cgi-bin/php");
// 	else if (ex.find(".py") != string::npos)
// 		return ("/usr/bin/python");
// 	return "NULL";
// }

char *ft_strdup(string msg)
{
	char *ret = static_cast<char*>(malloc(sizeof(char) * (msg.size() + 1)));

	unsigned int i = 0;
	while (i < msg.size())
	{
		ret[i] = msg[i];
		i++;
	}
	ret[i] = 0;
	return ret;
}

void	Request::launch_cgi(string &body, string extention_name)
{
	// look: https://github.com/brokenfiles/webserv/blob/c1601dfad39a04299bc86b165994a87f3146d78d/srcs/classes/cgi/Cgi.cpp addMetaVariables
	DEBUG("launch_cgi");
	// DEBUG("BODY IS " << body);
	int fdpipe[2];
	if (pipe(fdpipe) == -1)
	{
		cerr << "cgi: pipe failed" << endl;
		exit(EXIT_FAILURE);
	}
	pid_t pid = fork();
	if (pid < 0)
	{
		cerr << "cgi: fork failed" << endl;
		exit(EXIT_FAILURE);
	}

	code = 200;
	//https://stackoverflow.com/questions/33052169/call-php-cgi-from-c-with-request-method-post
	if (pid == 0)
	{
		string server_root = string(getcwd(NULL, 0));
		// envp[0] = 0;
		DEBUG("BODYIS " << headers["Body"]);
		// DEBUG("OR " << body);

		// envp[1] = ft_strdup("CONTENT_LENGHT=" + to_string(headers["Body"].length()));
		
		// //This is wrong it seems
		

		// envp[0] = ft_strdup(("DOCUMENT_ROOT=" + server_root));
		// // envp[1] = ft_strdup(("HTTP_HOST=" + (server->get_server_names().front())));
		// envp[2] = ft_strdup(("SCRIPT_FILENAME=" + server_root + "/" + filepath));
		// envp[3] = ft_strdup(("SCRIPT_NAME=" + filepath.substr(filepath.find_last_of('/')+ 1)));
		// envp[4] = ft_strdup(("PATH=" + server_root + "/"));
		// envp[5] = ft_strdup(("PATH_INFO=" + server_root + "/" + filepath));
		// envp[6] = 0;
		
		char **argv = (char**) malloc(sizeof(char*) * 4);
		char **envp = (char**) malloc(sizeof(char*) * 8);

		envp[0] = ft_strdup("GATEWAY_INTERFACE=CGI/1.1");
		envp[1] = ft_strdup("SERVER_PROTOCOL=HTTP/1.1");
		envp[2] = ft_strdup("REDIRECT_STATUS=200");

		if (type == "GET")
		{
			envp[3] = ft_strdup("QUERY_STRING="+ filepath.substr(filepath.find_first_of('?') + 1));
			envp[4] = ft_strdup("REQUEST_METHOD=GET");
			envp[5] = ft_strdup("SCRIPT_FILENAME=" + server_root + "/" + filepath.substr(0, filepath.find_first_of('?', 0)));
			envp[6] = ft_strdup("CONTENT_LENGTH=0");//+ to_string(headers["Body"].length()) );
			extention_name = extention_name.substr(0, extention_name.find_first_of('?'));
		}
		else if (type == "POST")
		{
			//To change according to content type
			envp[3] = ft_strdup("CONTENT_TYPE=application/x-www-form-urlencoded;charset=utf-8");		
			envp[4] = ft_strdup("REQUEST_METHOD=POST");
			envp[6] = ft_strdup("CONTENT_LENGTH="+ to_string(headers["Body"].length()) );
			envp[5] = ft_strdup("SCRIPT_FILENAME=" + server_root + "/" + filepath);
		}
		else
		{
			DEBUG(type);
		}
		envp[7] = 0;
		// envp[5] = ft_strdup(("SCRIPT_FILENAME=" + server_root + "/" + filepath.substr(0, filepath.find_first_of('?', 0))));
		// envp[6] = ft_strdup("CONTENT_LENGTH="+ to_string(headers["Body"].length()) );
		
		// envp[4] = ft_strdup("REQUEST_METHOD=POST");		


		
		//TODO
		// envp[2] = ft_strdup("QUERY_STRING="+ filepath.substr(filepath.find_first_of('?') + 1));
		// envp[2] = ft_strdup("QUERY_STRING=name=enzo");

		// envp[3] = ft_strdup("REDIRECT_STATUS=200");
		// envp[4] = ft_strdup("REQUEST_METHOD=POST");

		// envp[5] = ft_strdup("CONTENT_TYPE=application/x-www-form-urlencoded;charset=utf-8");


		// envp[5] = ft_strdup(("SCRIPT_FILENAME=" + server_root + "/" + filepath.substr(0, filepath.find_first_of('?', 0))));
		// envp[6] = ft_strdup("CONTENT_LENGTH="+ to_string(headers["Body"].length()) );
		
		// env_vector.push_back("GATEWAY_INTERFACE=CGI/1.1");
		// env_vector.push_back("SERVER_PROTOCOL=HTTP/1.1");
		// env_vector.push_back("QUERY_STRING=name=enzo");
		// env_vector.push_back("REDIRECT_STATUS=200");
		// env_vector.push_back("REQUEST_METHOD=POST");
		// env_vector.push_back("CONTENT_TYPE=application/x-www-form-urlencoded;charset=utf-8");
		// env_vector.push_back(("SCRIPT_FILENAME=" + server_root + "/" + filepath));
		// env_vector.push_back("CONTENT_LENGTH="+ to_string(sData.length()) );

		// for (int i =0; i < 6; i++)
		// {
		// 	DEBUG(i << ":" << envp[i]);
		// }
		// envp[] = &("HTTP_USER_AGENT=" + tmp)[0];
		// envp[] = &("HTTPS=" + tmp)[0];

		string bin_path = server->get_cgis()[extention_name];
		// string bin_path = "./website/cgi-bin/php-cgi";
		// DEBUG("EXAME PATH " << extention_name);
		argv[0] = ft_strdup(bin_path);
		// argv[1] = ft_strdup(server_root + "/" + filepath);
		argv[1] = ft_strdup(server_root + "/" + filepath.substr(0, filepath.find_first_of('?', 0)));
		argv[2] = ft_strdup(headers["Body"]);
		argv[3] = 0;
		// argv[4] = 0;
		for (int i =0; i < 3; i++)
		{
			DEBUG(i << "A!:" << argv[i]);
		}
		for (int i =0; i < 7; i++)
		{
			DEBUG(i << "E!:" << envp[i]);
		}
		DEBUG("DONE");
		//TODO: calculate size of envp and build it
		
		//Pipes don't do much at this point
		
		close(fdpipe[0]); // child doesn't read
		dup2(fdpipe[1], STDOUT_FILENO);

		
		// TODO: must execve php-cgi
		if (execve(bin_path.c_str(), argv, envp) < 0)
			code = 404;
		//Need to free
		DEBUG("EXECVE DONE");
	}
	else
	{
		wait(0);
		// waitpid(0, NULL, 0);
		// size_t i = 0;
		// while (argv[i])
		// {
		// 	DEBUG(i);
		// 	DEBUG(argv[i]);
		// 	free(argv[i++]);
		// }
		// i = 0;
		// while (envp[i])
		// 	free(envp[i++]);
		// free(argv);
		// free(envp);
		close(fdpipe[1]); // parent doesn't write
		char reading_buf;
		while(read(fdpipe[0], &reading_buf, 1) > 0)
		   body += reading_buf;
		DEBUG("qwerty:" << body);
		close(fdpipe[0]);
	}
}

void	Request::get_auto_index(string &body)
{
	stringstream auto_index;
	passed_cgi = true;
	auto_index << "	<html lang=\"en\">\n\
					<body style=\"background-color: grey; color: lightgrey;\">\n\
					<div style=\"display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100%;\">\n\
						<h1>Auto Index</h1>\n";
	auto_index << filepath.substr(server->get_root().length() + 1);
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(filepath.c_str())) != NULL)
	{
		while ((ent = readdir (dir)) != NULL)
		{
			string name = ent->d_name;
			ifstream file(name.c_str(), ofstream::in);
			if (!file || !file.is_open() || !file.good() || file.fail() || file.bad())
			{
				// todo exit ?
			}
			if (is_directory(filepath + name))
				name += '/';
			file.close();
			auto_index << "<p><a href=\"" << name << "\" class=\"active\">" << name << "</a></p>\n";
		}
		closedir (dir);
	}
	else
	{
		perror("opendir");
		exit(EXIT_FAILURE);
	}
	auto_index << "	</div>\n\
					</body>\n\
					</html>";
	code = 200;
	body = auto_index.str();
}

void	Request::set_filepath(void)
{
	if (!server || !location)
	{
		filepath = "";
		return;
	}
	filepath = server->get_root();
	if (target.compare("/") == 0)
	{
		filepath += '/' + server->get_index();
		return ;
	}
	size_t pos = target.find(location->get_path());
	if (pos == 0)
	{
		string tmp = target.substr(pos + location->get_path().length());
		filepath += location->get_location_root() + tmp;
	}
	else
		filepath += target;
	if (filepath[filepath.length() - 1] == '/')
	{
		DEBUG("Target is a DIRECTORY !");
		if (location->get_index().length())
			filepath += location->get_index();
		else
		{
			// 	filepath += server->get_index();
		}
	}
	DEBUG("filepath: " << filepath << endl << endl);
}

void 	Request::get_body(string &body)
{
	set_filepath();
	ifstream file(filepath.c_str(), ofstream::in);
	if (is_directory(filepath))
	{
		if (filepath[filepath.size() - 1] == '/' && location->get_autoindex())
			return (get_auto_index(body));
		else
		{
			code = 403;
			file.close();
			file.open(static_cast<const char *>(error_page(403).c_str()), ofstream::in);
		}
	}
	if (filepath.length() && get_type(filepath, false) != "text/html")
	{

		DEBUG("CGI SEARCHED");
		map<string, string> cgis = server->get_cgis();
		for (map<string, string>::iterator cgi = cgis.begin(); cgi != cgis.end(); cgi++)
		{
			size_t pos;
			if ((pos = filepath.find(cgi->first) )!= string::npos)
			{
				DEBUG("CGI extention found !");
				passed_cgi = true;
				file.close();
				DEBUG("BODY B4 CGI " << body)
				launch_cgi(body, filepath.substr(pos));
				return ;
			}
		}
		DEBUG("CGI NOT FOUND");
	}
	if (!file || !file.is_open() || !file.good() || file.fail() || file.bad()) // || file_is_empty(file))
	{
		code = 404;
		file.close();
		file.open(static_cast<const char *>(error_page(404).c_str()), ofstream::in);
	}
	else if (code == 0)
		code = 200;
	body = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	file.close();
}

string 	Request::get_response(void)
{
	string response;
	string body;
	get_body(body);
	response = get_header(body.length());
	response += body;
	// if (type == "text/html")
	// {
	// 	DEBUG("********* RESPONSE *********");
	// 	DEBUG(response);
	// }
	// DEBUG("@@@@@@@@@@@@@@@@@@ END @@@@@@@@@@@@@@@@@@");
	return response;
}

bool	Request::select_location(void)
{
	list<Location> locations = server->get_locations();
	if (locations.size() == 0)
	{
		location = NULL;
		return false;
	}
	string searched_path = target;
	while (searched_path != "")
	{
		// DEBUG(searched_path);
		for (list<Location>::iterator it_location = locations.begin(); it_location != locations.end(); it_location++)
		{
			if (searched_path == it_location->get_path())
			{
				if (this->location != NULL)
				{
					DEBUG("ERASING POINTER");
				}
				else
				{
					DEBUG("PROPER ASSING");
				}
				this->location = new Location(*it_location);
				return true;
			}
		}
		size_t pos = searched_path.find_last_of("/");
		if (pos == string::npos)
			break;
		searched_path = searched_path.substr(0, pos);
	}
	searched_path = "/";
	for (list<Location>::iterator it_location = locations.begin(); it_location != locations.end(); it_location++)
	{
		if (searched_path == it_location->get_path())
		{
			if (this->location != NULL)
			{
				DEBUG("ERASING POINTER");
			}
			else
			{
				DEBUG("PROPER ASSING");
			}
			this->location = new Location(*it_location);
			return true;
		}
	}
	this->location = NULL;
	return false;
}

string	Request::get_header(const size_t length)
{
	stringstream header;
	header << "HTTP/1.1 " << codes[code] << endl;
	header << "Date: " << get_time_stamp() << endl;
	header << "Server: webserv/0.01" << endl;
	header << "Content-Type: " << get_type(filepath, passed_cgi) << endl;
	header << "Content-Length: " << length << endl;
	header << "Connection: Closed" << endl;
	if (location && location->get_HTTP_redirection_type() > 0)
		header << "Location: " << location->get_HTTP_redirection() << endl;
	header << endl;
	return (header.str());
}

bool	Request::method_allow(void)
{
	list<string> allow = location->get_allow();
	for (list<string>::iterator HTTP_method = allow.begin(); HTTP_method != allow.end(); HTTP_method++)
	{
		if (*HTTP_method == type)
			return true;
	}
	code = 405;
	return false;
}

string	Request::respond(const list<Server*> &servers)
{
	if (!select_server(servers) || !select_location() || !method_allow())
		return (get_response());
	if (((unsigned int) atoi(headers["Content-Length"].c_str())) > server->get_max_client_body_size())
	{
		code = 413;
		return (get_response());
	}
	if (location->get_HTTP_redirection_type() > 0)
	{
		code = location->get_HTTP_redirection_type();
		return (get_header(0));
	}
	if (type == "GET")
		return(get_response());
	else if (type == "HEAD")
	{
		string body;
		get_body(body);
		return (get_header(body.length()));
	}
	else if (type == "POST")
	{
		return (get_response());
	}
	else if (type == "DELETE")
	{
		//todo
		string delete_file = server->get_root() + location->get_upload_directory() + target;
		remove(delete_file.c_str());
		return (get_response());
	}
	return (get_response());
}
