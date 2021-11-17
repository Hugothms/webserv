/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 17:21:43 by hthomas           #+#    #+#             */
/*   Updated: 2021/11/17 15:34:33 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Request::~Request() {}

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

string	getdayofweek(const int day)
{
	switch(day)
	{
		case 1:
			return "Mon";
		case 2:
			return "Tue";
		case 3:
			return "Wed";
		case 4:
			return "Thu";
		case 5:
			return "Fri";
		case 6:
			return "Sat";
		case 0:
			return "Sun";
		default :
			return "UNDEFINED_DAY";
	}
}

string	getmonth(const int month)
{
	switch(month)
	{
		case 0:
			return "Jan";
		case 1:
			return "Feb";
		case 2:
			return "Mar";
		case 3:
			return "Apr";
		case 4:
			return "May";
		case 5:
			return "Jun";
		case 6:
			return "Jul";
		case 7:
			return "Aug";
		case 8:
			return "Sep";
		case 9:
			return "Oct";
		case 10:
			return "Nov";
		case 11:
			return "Dec";
		default :
			return "UNDEFINED_MONTH";
	}
}

string	get_time_stamp()
{
	time_t now = time(0);

	tm *time = gmtime(&now);
	stringstream output;
	output << getdayofweek(time->tm_wday);

	if (time->tm_mday < 10)
		output  << ", 0" << time->tm_mday;
	else
		output  << ", " << time->tm_mday;
	output << " " << getmonth(time->tm_mon);
	output << " " << time->tm_year + 1900 << " ";
	if (time->tm_hour < 10)
		output << "0";
	output << time->tm_hour << ":";
	if (time->tm_min < 10)
		output << "0";
	output << time->tm_min << ":";
	if (time->tm_sec < 10)
		output << "0";
	output << time->tm_sec << " GMT";

	return (output.str());
}

string	get_type(const string &str)
{
	string ret;
	if (str.find(".png", str.length() - 4) != string::npos)
		ret = "image/png";
	else if (str.find(".jpg", str.length() - 4) != string::npos)
		ret = "image/jpg";
	else if (str.find(".png", str.length() - 4) != string::npos)
		ret = "image/x-icon";
	else if (str.find(".ico", str.length() - 4) != string::npos)
		ret = "image/x-icon";
	else if (str.find(".css", str.length() - 4) != string::npos)
		ret = "text/css";
	else if (str.find(".js", str.length() - 3) != string::npos)
		ret = "application/javascript";
	else if (str.find(".html", str.length() - 5) != string::npos)
		ret = "text/html";
	else
		ret = "text/html";
	return ret;
}

string	error_page(const Server *server, const int x)
{
	if (server && server->get_error_pages().size() && server->get_error_pages()[x].length())
			return (server->get_root() + server->get_error_pages()[x]);
	return ("default_error_pages/" + to_string(x) + ".html");
}

/**
 * @param servers	list of server in which to search for the corresponding one
 * @param host		specified in the conf by the 'server_name' keyword
 * @param port		specified in the conf by the 'listen' keyword (ip_address:port)
 * @return			a pointer to the server corresponding to the couple (host, port) or the default server for this port if it exist, null overwise.
**/
Server	*select_server(const list<Server*> &servers, string &host, unsigned int port)
{
	// DEBUG("Looking for " << host << ":" << port);
	Server *default_server = NULL;
	for (list<Server*>::const_iterator server = servers.begin(); server != servers.end(); server++)
	{
		if ((*server)->get_port() == port)
		{
			if (default_server == NULL)
				default_server = *server;
			list<string> server_names = (*server)->get_server_names();
			for (list<string>::iterator server_name = server_names.begin(); server_name != server_names.end(); server_name++)
			{
				// DEBUG("Candidate " << *server_name << ":" << (*server)->get_port());
				if ((*server_name == "0.0.0.0" || *server_name == host) && (*server)->get_port() == port)
				{
					// DEBUG("Found " << *server_name << ":" << (*server)->get_port());
					return (*server);
				}
			}
		}
	}
	// if (default_server)
		// DEBUG("Found default server for port: " << port);
	return default_server;
}

string get_header(const string &message, const string &type, const size_t length)
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

void launch_cgi(const Server *server, string &message, string filepath, string &body)
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
	if (pid == 0)
	{
		string server_root = string(getcwd(NULL, 0));
		char *file = strdup((server_root + "/" + filepath).c_str());
		char **argv = (char**) malloc(sizeof(char*) * 2);
		argv[0] = file;
		argv[1] = 0;
		//TODO: calculate size of envp and build it
		char **envp = (char**) malloc(sizeof(char*) * 6);
		// envp[0] = malloc()
		envp[0] = strdup(("DOCUMENT_ROOT=" + server_root).c_str());
		envp[1] = strdup(("HTTP_HOST=" + (server->get_server_names().front())).c_str());
		envp[2] = strdup(("SCRIPT_FILENAME=" + server_root + filepath).c_str());
		envp[3] = strdup(("SCRIPT_NAME=" + filepath.substr(filepath.find_last_of('/')+ 1)).c_str());
		envp[4] = strdup(("PATH=" + server_root).c_str());
		envp[5] = 0;
		// envp[4] = &("HTTP_USER_AGENT=" + tmp)[0];
		// envp[5] = &("HTTPS=" + tmp)[0];
		message = CODE_200;
		// TODO: pipe STDOUT into BODY ?
		close(fdpipe[0]); // child doesn't read
		dup2(STDOUT_FILENO, fdpipe[1]);
		execve("/usr/bin/python", argv, envp);
		free(argv);
		free(envp);
		exit(EXIT_SUCCESS);
	}
	else
	{
		wait(0);
		close(fdpipe[1]); // parent doesn't write
		char reading_buf[1];
        while(read(fdpipe[0], reading_buf, 1) > 0)
        {
           cout << reading_buf[0];
		   body += reading_buf;
        }
        close(fdpipe[0]);
	}

}

void get_auto_index(const Server *server, string &message, string filepath, string &body)
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

void 	get_body(const Server *server, const Location *location, string &message, string filepath, string &body)
{
	if (location->get_directory_listing() && is_directory(filepath) && filepath.back() == '/')
		return (get_auto_index(server, message, filepath, body));
	list<string> cgis = server->get_cgis();
	for (list<string>::iterator cgi = cgis.begin(); cgi != cgis.end(); cgi++)
	{
		if (filepath.find(*cgi) != string::npos)
		{
			DEBUG("CGI extention found !");
			return (launch_cgi(server, message, filepath, body));
		}
	}
	ifstream file(filepath.c_str(), ofstream::in);
	if (!file || !file.is_open() || !file.good() || file.fail() || file.bad() || file_is_empty(file))
	{
		message = CODE_404;
		file.close();
		file.open(error_page(server, 404), ofstream::in);
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

string 	get_response(const Server *server, const Location *location, string message, string filepath)
{
	string response;
	string body;
	get_body(server, location, message, filepath, body);
	response = get_header(message, get_type(filepath), body.length());
	response += body;
	// if (type == "text/html")
	// {
	// 	DEBUG("********* RESPONSE *********");
	// 	DEBUG(response);
	// }
	// DEBUG("@@@@@@@@@@@@@@@@@@ END @@@@@@@@@@@@@@@@@@");
	return response;
}

Location	*select_location(const Server *server, const string &target)
{
	list<Location> locations = server->get_locations();
	if (locations.size() == 0)
		return NULL;
	string searched_path = target;
	// DEBUG("tmp: " << target);
	while (searched_path != "")
	{
		// DEBUG(searched_path);
		for (list<Location>::iterator location = locations.begin(); location != locations.end(); location++)
		{
			// DEBUG("searched_path: " << searched_path << "\t\t" << "path: " << (*location).get_path());
			if (searched_path == location->get_path())
			{
				// DEBUG("Location found: " << location->get_path());
				return ((new Location(*location)));
			}
		}
		size_t pos = searched_path.find_last_of("/");
		if (pos == string::npos)
			break;
		searched_path = searched_path.substr(0, pos);
	}
	searched_path = "/";
	for (list<Location>::iterator location = locations.begin(); location != locations.end(); location++)
	{
		if (searched_path == location->get_path())
		{
			// DEBUG("Default location found: " << location->get_path());
			return ((new Location(*location)));
		}
	}
	return NULL;
}

bool method_allowed(const Location *location, const string &method)
{
	list<string> HTTP_methods = location->get_HTTP_methods();
	for (list<string>::iterator HTTP_method = HTTP_methods.begin(); HTTP_method != HTTP_methods.end(); HTTP_method++)
	{
		if (*HTTP_method == method)
			return true;
	}
	return false;
}

void Request::set_filepath()
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
	server = select_server(servers, headers["Host"], atoi(headers["Port"].c_str()));
	if (!server)
		return (get_response(server, location, CODE_404, error_page(server, 404)));
	location = select_location(server, target);
	if (!location)
		return (get_response(server, location, CODE_404, error_page(server, 404)));
	if (!method_allowed(location, type))
		return (get_response(server, location, CODE_405, error_page(server, 405)));
	set_filepath();
	string message;
	if (type == "GET")
		return(get_response(server, location, "", filepath));
	else if (type == "HEAD")
	{
		string body;
		get_body(server, location, message, filepath, body);
		return (get_header(message, get_type(filepath), body.length()));
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
		return (get_response(server, location, "", filepath));
	}
	else if (type == "DELETE")
	{
		//todo
		string delete_file = server->get_root() + location->get_upload_directory() + target;
		remove(delete_file.c_str());
		return (get_response(server, location, "", filepath));
	}
	return (get_response(server, location, CODE_405, error_page(server, 405)));
}
