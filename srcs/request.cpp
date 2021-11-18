/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 17:21:43 by hthomas           #+#    #+#             */
/*   Updated: 2021/11/18 14:49:06 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Request::~Request()
{}

Request::Request(const string &buffer)
{
	size_t pos = 0;


	DEBUG(endl << endl << "******* NEW REQUEST: ********");
	DEBUG(buffer);

	// string buffer(buffer, size);
	type = get_str_before_char(buffer, " ", &pos);
	target = get_str_before_char(buffer, " ", &pos);
	get_str_before_char(buffer, "\n", &pos);

	size_t len = buffer.length();
	while (pos <= len && buffer[pos]) // headers parsing loop
	{
		if (buffer[pos] == '\n')
			pos++;
		string header = get_str_before_char(buffer, ":\n", &pos);
		// DEBUG((int)header[0] << "/" << (int)header[1] << "\t|" << header << "|");
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
			// if (ip_address == "localhost")
			// 	ip_address = "127.0.0.1";
			headers.insert(pair<string, string>("Host", ip_address));
			headers.insert(pair<string, string>("Port", port));
			continue;
		}
		headers.insert(pair<string, string>(header, get_str_before_char(buffer, "\r\n", &pos)));
	}
	if (headers.count("Content-Length") > 0)
		headers.insert(pair<string, string>("Body", &buffer[pos]));
	// for (map<string, string>::iterator it = headers.begin(); it != headers.end(); it++)
	// 	DEBUG(it->first << ": " << it->second);
	// DEBUG("****** REQUEST PARSED *******");
}

string	Request::error_page(const int error_code)
{
	if (server && server->get_error_pages().size() && server->get_error_pages()[error_code].length())
			return (server->get_root() + server->get_error_pages()[error_code]);
	return ("default_error_pages/" + to_string(error_code) + ".html");
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
	// DEBUG("Looking for " << host << ":" << port);
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
				// DEBUG("Candidate " << *server_name << ":" << (*server)->get_port());
				if ((*server_name == "0.0.0.0" || *server_name == host) && (*server)->get_port() == port)
				{
					// DEBUG("Found " << *server_name << ":" << (*server)->get_port());
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

string get_bin(char *path)
{
	string ex(path);


	if (ex.find(".php") != string::npos)
		return ("/usr/bin/php");
	else if (ex.find(".py") != string::npos)
		return ("/usr/bin/python");
	return "NULL";
}

void Request::launch_cgi(string &body)
{
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
	char **argv = (char**) malloc(sizeof(char*) * 4);
	char **envp = (char**) malloc(sizeof(char*) * 6);
	if (pid == 0)
	{
		string server_root = string(getcwd(NULL, 0));
		envp[0] = strdup(("DOCUMENT_ROOT=" + server_root).c_str());
		envp[1] = strdup(("HTTP_HOST=" + (server->get_server_names().front())).c_str());
		envp[2] = strdup(("SCRIPT_FILENAME=" + server_root + "/" + filepath).c_str());
		envp[3] = strdup(("SCRIPT_NAME=" + filepath.substr(filepath.find_last_of('/')+ 1)).c_str());
		envp[4] = strdup(("PATH=" + server_root +"/").c_str());
		envp[5] = 0;
		// envp[4] = &("HTTP_USER_AGENT=" + tmp)[0];
		// envp[5] = &("HTTPS=" + tmp)[0];

		string bin_path = get_bin(envp[3]);

		argv[0] = strdup(bin_path.c_str());
		argv[1] = strdup((server_root + "/" + filepath).c_str());
		argv[2] = strdup(headers["Body"].c_str());
		argv[3] = 0;
		//TODO: calculate size of envp and build it
		message = CODE_200;
		close(fdpipe[0]); // child doesn't read
		dup2(fdpipe[1], STDOUT_FILENO);
		execve(bin_path.c_str(), argv, envp);
	}
	else
	{
		wait(0);
		// waitpid(pid, &child_status, 0);
		size_t i = 0;
		// while (argv[i])
		// {
		// 	DEBUG(i);
		// 	DEBUG(argv[i]);
		// 	free(argv[i++]);
		// }
		// i = 0;
		// while (envp[i])
		// 	free(envp[i++]);
		free(argv);
		free(envp);
		close(fdpipe[1]); // parent doesn't write
		char reading_buf;
		DEBUG("Body:");
		while(read(fdpipe[0], &reading_buf, 1) > 0)
		{
		   cout << reading_buf;
		   body += reading_buf;
		}
		close(fdpipe[0]);
	}
}

void	Request::get_auto_index(string &body)
{
	stringstream auto_index;
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
		perror("");
		exit(EXIT_FAILURE);
	}
	auto_index << "	</div>\n\
					</body>\n\
					</html>";
	message = CODE_200;
	body = auto_index.str();
}

void 	Request::get_body(string &body)
{
	if (location->get_directory_listing() && is_directory(filepath) && filepath.back() == '/')
		return (get_auto_index(body));
	list<string> cgis = server->get_cgis();
	for (list<string>::iterator cgi = cgis.begin(); cgi != cgis.end(); cgi++)
	{
		if (filepath.find(*cgi) != string::npos)
		{
			DEBUG("CGI extention found !");
			return (launch_cgi(body));
		}
	}
	ifstream file(filepath.c_str(), ofstream::in);
	if (!file || !file.is_open() || !file.good() || file.fail() || file.bad() || file_is_empty(file))
	{
		message = CODE_404;
		file.close();
		file.open(error_page(404), ofstream::in);
	}
	else if (message == "")
		message = CODE_200;
	body = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	// if (body == "") // if file is a directory or is empty
	// {
	// 	message = CODE_404;
	// 	file.close();
	// 	file.open(error_page(server, 404), ofstream::in);
	// 	body = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	// }
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
	// DEBUG("tmp: " << target);
	while (searched_path != "")
	{
		// DEBUG(searched_path);
		for (list<Location>::iterator it_location = locations.begin(); it_location != locations.end(); it_location++)
		{
			// DEBUG("searched_path: " << searched_path << "\t\t" << "path: " << (*it_location).get_path());
			if (searched_path == it_location->get_path())
			{
				// DEBUG("Location found: " << location->get_path());
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
			// DEBUG("Default it_location found: " << it_location->get_path());
			this->location = new Location(*it_location);
			return true;
		}
	}
	this->location = NULL;
	return false;
}

string Request::get_header(const size_t length)
{
	stringstream header;
	header << "HTTP/1.1 " << message << endl;
	header << "Date: " << get_time_stamp() << endl;
	header << "Server: webserv/0.01" << endl;
	header << "Content-Type: " << type << endl;
	header << "Content-Length: " << length << endl;
	header << "Connection: Closed" << endl;
	header << endl;
	return (header.str());
}

bool Request::method_allowed(void)
{
	list<string> HTTP_methods = location->get_HTTP_methods();
	for (list<string>::iterator HTTP_method = HTTP_methods.begin(); HTTP_method != HTTP_methods.end(); HTTP_method++)
	{
		if (*HTTP_method == type)
			return true;
	}
	return false;
}

void Request::set_filepath(void)
{
	filepath = server->get_root();
	if (target.compare("/") == 0)
	{
		filepath += '/' + server->get_index();
		return ;
	}
	size_t pos = target.find(location->get_path());
	DEBUG("filepath: " << filepath);
	DEBUG("target: " << target);
	DEBUG("path(): " << location->get_path());
	DEBUG("root(): " << location->get_location_root());
	DEBUG("pos:" << pos);
	DEBUG("");
	if (pos == 0)
	{
		string tmp = target.substr(pos + location->get_path().length());
		// string target_copy = string(target).resize(pos);
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

string	Request::respond(const list<Server*> &servers)
{
	if (!select_server(servers) || !select_location() || !method_allowed())
		return (get_response());
	set_filepath();
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
		// if (is_file_upload())
		// {
			// string upload_dir = server->get_root() + location->get_upload_directory();
			// mkdir(upload_dir.c_str(), S_IRWXU|S_IRWXG|S_IRWXO);
			// string upload_file = upload_dir + target;
			// DEBUG("CREATE this file: " << upload_file);
			// ofstream file_out(upload_file, ios::app);
			// file_out << headers["Body"] << endl;
			// file_out.close();
			// return (get_response(server, location, "", "success.html"));
		// }
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
