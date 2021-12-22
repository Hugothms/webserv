/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 17:21:43 by hthomas           #+#    #+#             */
/*   Updated: 2021/12/22 23:37:26 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"
#include <fcntl.h>
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
	if (this->location != 0)
		delete(this->location);
	// free(static_cast<void *>(this->location));
}

string Request::g_type(void) const
{
	return type;
}

Request::Request(const string &buffer)
:  code(0), passed_cgi(false), location(0)
{
	size_t pos = 0;

	DEBUG(endl << endl << "******* NEW REQUEST BUFF: ********\n");
	DEBUG(buffer);
	DEBUG("******* END OF REQUEST BUFF ********\n");

	size_t t_pos = buffer.find("Content-Type: ");
	if (t_pos != string::npos)
	{
		content_type = buffer.substr(t_pos, buffer.find('\n', t_pos));
		// DEBUG("TYPE IS " << content_type);
		t_pos = content_type.find_first_of(": ") + 2;
		content_type = content_type.substr(t_pos, (content_type.find("\n", 0) - t_pos));
		// if (!content_type.empty())
		// DEBUG("CONTENT TYPE IS" << content_type << "/////////////");		
	}


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
	DEBUG("****** REQUEST PARSED *******");
	// DEBUG("Done")
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
	// DEBUG("Found default server for port: " << port);
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
string trim_tr(string to_trim)
{
	string todo;
	for (std::string::iterator it = to_trim.begin(); it != to_trim.end(); it++)
	{
		if (*it == 13)
		{
			// DEBUG("DO TRIM");
			it = to_trim.erase(it);
			--it;
			continue;
		}
	}	
	return to_trim;
}

char **Request::build_cgi_av(string &extention_name)
{
	std::vector<string> av;
	char *cwd = getcwd(NULL, 0);
	string server_root = string(cwd);
	free(cwd);
	string bin_path = server->get_cgis()[extention_name];

	av.push_back(bin_path);
	av.push_back(server_root + "/" + filepath.substr(0, filepath.find_first_of('?', 0)));

	char **_av = static_cast<char**>(malloc(sizeof(char *) * (av.size() + 1)));
		
	for (size_t j = 0; j < av.size(); j++)
	{
		_av[j] = ft_strdup(av[j]);
		DEBUG("a"<<j << ":" << av[j]<<"|");
	}
	_av[av.size()] = 0;
	return _av;
}

char **Request::build_cgi_env(string &extention_name)
{
	std::vector<string> ev;

	char *cwd = getcwd(NULL, 0);
	string server_root = string(cwd);
	free(cwd);
	string newfilepath("/" + filepath.substr(0, filepath.find_first_of('?', 0)));


	ev.push_back("GATEWAY_INTERFACE=CGI/1.1");
	ev.push_back("SERVER_PROTOCOL=HTTP/1.1");
	ev.push_back("REDIRECT_STATUS=200");
	ev.push_back("HTTP_HOST=" + server_root + newfilepath);
	ev.push_back("REQUEST_METHOD=" + type);
	ev.push_back("SCRIPT_FILENAME=" + server_root + newfilepath);
	ev.push_back("SCRIPT_NAME=" + newfilepath);


	if (type == "GET")
	{
		ev.push_back("QUERY_STRING="+ filepath.substr(filepath.find_first_of('?') + 1));
		ev.push_back("CONTENT_LENGTH=0");//+ to_string(headers["Body"].length()) );
		extention_name = extention_name.substr(0, extention_name.find_first_of('?'));
	}
	else if (type == "POST")
	{

		// DEBUG("TYPE IS:" << content_type);



		// content_type = content_type.substr(0, content_type.find_first_of('\n'));
		// content_type.append(";charset=utf-8");
		
		
		// DEBUG("TYPE IS:" << content_type << '|');

		// write(2, content_type.c_str(), content_type.size());

		// DEBUG(content_type);

		//Are you fucking with me ?
		content_type = trim_tr(content_type);
		ev.push_back("CONTENT_TYPE=" + content_type);
		
		// for (int i = 0; i < content_type.size(); i++)
		// {
		// 	DEBUG(i<<"|"<< static_cast<int>(content_type[i])<<":"<<content_type[i]<<"|");
		// }

		ev.push_back("CONTENT_LENGTH="+ to_string_custom(headers["Body"].length()));//(data_buff->length()) );
	}

	char **_ev = static_cast<char**>(malloc(sizeof(char *) * (ev.size() + 1)));
		
	for (size_t j = 0; j < ev.size(); j++)
	{
		_ev[j] = ft_strdup(ev[j]);
		DEBUG("e"<<j << ":|" << ev[j]<<"|");
	}
	_ev[ev.size()] = 0;
	return _ev;
}





void	Request::launch_cgi(string &body, string extention_name)
{
	// look: https://github.com/brokenfiles/webserv/blob/c1601dfad39a04299bc86b165994a87f3146d78d/srcs/classes/cgi/Cgi.cpp addMetaVariables
	DEBUG("launch_cgi");
	// DEBUG("BODY IS " << body);
	

	int out_pipe[2];
	int in_pipe[2];


	//Create pipes for input and output
	if (pipe(out_pipe) == -1)
	{
		cerr << "cgi: pipe failed" << endl;
		exit(EXIT_FAILURE);
	}

	//We probably shouldn't assume that
	code = 200;
	
	//We need to trim and write the body to stdin
	if (type == "POST")
	{	

		// open("/tmp/webserv_cgi", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)
		

		// headers["Body"] = trim_tr(headers["Body"]);
		// headers["Body"] = headers["Body"].substr(0, headers["Body"].find_last_of('\n'));


		// int fd = open("testfile", O_WRONLY | O_CREAT | O_TRUNC, 0777);
		// if (fd == -1)
		// {
		// 	DEBUG("FILE CREATION ERRROR ");
		// 	exit(0);
		// }
		// write(fd, headers["Body"].c_str(), headers["Body"].size());
		// close(fd);
		if (pipe(in_pipe) == -1)
			DEBUG("PIPE ERROR");
		
	}
	
	//Get values for execve

	
	pid_t pid = fork();
	
	if (pid < 0)
	{
		cerr << "cgi: fork failed" << endl;
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{	


		char **_ev = build_cgi_env(extention_name);
		char **_av = build_cgi_av(extention_name);
	

		if (type == "POST")
		{
			// DEBUG("DUPPING OUT");
			close(in_pipe[1]);
			if (dup2(in_pipe[0], 0) == -1)
			{
				DEBUG("DUP2 ERR");
				exit(0);
			}
		}
		close(out_pipe[0]);
		if (dup2(out_pipe[1], 1) == -1)
		{
			DEBUG("DUP2 ERR");
			exit(0);
		}

		if (execve(_av[0], _av, _ev) < 0)
			code = 404;
		//Need to free
	}
	else
	{
		// for (int i = 0)


		close(out_pipe[1]);
		if (type == "POST")
		{
			close(in_pipe[0]);
			DEBUG("POST TREATMENT");
			// DEBUG(headers["Body"].size() << ":"<< headers["Body"] << '|');

			if (write(in_pipe[1], headers["Body"].c_str(), headers["Body"].size()) < 0)
				DEBUG("WRITE ERROR");
			close(in_pipe[1]);
			DEBUG("WRITE DONE");
		}
		// else
		// {

		// 	DEBUG("TST" << ":"<<"sample=text" << '|');
		// 	close(in_pipe[0]);

		// 	if (write(in_pipe[1], "sample=text", 11) < 0)
		// 		DEBUG("WRITE ERROR");
		// 	close(in_pipe[1]);		
		// 	DEBUG("WRITE DONE");
		// }
		wait(0);
		// close(in_pipe[1]);		
		// parent doesn't write 
		
		char reading_buf;
		while(read(out_pipe[0], &reading_buf, 1) > 0)
		   body += reading_buf;
		
		close(out_pipe[0]);


		DEBUG("CGI OUTPUT:\n" << body);
		
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
	// DEBUG("filepath: " << filepath << endl << endl);
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

		// DEBUG("CGI SEARCHED");
		map<string, string> cgis = server->get_cgis();
		for (map<string, string>::iterator cgi = cgis.begin(); cgi != cgis.end(); cgi++)
		{
			size_t pos;
			if ((pos = filepath.find(cgi->first) ) != string::npos)
			{
				passed_cgi = true;
				file.close();
				launch_cgi(body, filepath.substr(pos));
				return ;
			}
		}
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
	header << "Content-Type: " << ::get_type(filepath, passed_cgi) << endl;
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
	// if(data != 0)
	// {
	// 	data_buff = data;
	// 	//Do stuff
	// }
	if (!select_server(servers) || !select_location() || !method_allow())
		return (get_response());
	if (((unsigned int) atoi(headers["Content-Length"].c_str())) > server->get_max_client_body_size())
	{
		code = 413;
		return (get_response());
	}
	if (location->get_HTTP_redirection_type() > 0)
	{
		DEBUG("REDIR TYPE");
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
