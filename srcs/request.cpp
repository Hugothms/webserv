/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 16:29:23 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/30 22:08:38 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "request.hpp"

Request::~Request() {}

Request::Request(const char *buffer, const size_t size, const int sock)
: socket(sock)//, content_length(0), port(0)
{
	DEBUG(endl << endl << "******* NEW REQUEST: ********");
	DEBUG(buffer);
	size_t pos = 0;
	string request(buffer, size);
	type = get_str_before_char(request, " ", &pos);
	target = get_str_before_char(request, " ", &pos);
	get_str_before_char(request, "\n", &pos);
	string header;
	while (pos < size && request[pos]) // headers parsing loop
	{
		header = get_str_before_char(request, ": ", &pos);
		// DEBUG((int)header[0] << "/" << (int)header[1] << "\t|" << header << "|");
		if (header == "\0")
			break ; // case empty line
		if (header == "Host")
		{
			string ip_address;
			string port;
			if ((ip_address = get_str_before_char(request, ":", &pos)).length())
				port = get_str_before_char(request, "\n", &pos);
			else
			{
				ip_address = get_str_before_char(request, "\n", &pos);
				port = "80";
			}
			// if (ip_address == "localhost")
			// 	ip_address = "127.0.0.1";
			headers.insert(pair<string, string>("Host", ip_address));
			headers.insert(pair<string, string>("Port", port));
			continue;
		}
		headers.insert(pair<string, string>(header, get_str_before_char(request, "\n", &pos)));
	}
	pos++;
	if (headers.count("Content-Length"))
		headers.insert(pair<string, string>("Body", &request[pos]));
	DEBUG("****** PARSING REQUEST ******");
	DEBUG("type:" << type);
	DEBUG("target:" << target);
	DEBUG("socket:" << socket);
	map<string, string>::iterator it = headers.begin();
	while(it != headers.end())
		DEBUG(it->first << ": " << it++->second);
	DEBUG("****** REQUEST PARSED *******");
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
			return "Now";
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

string	get_type(const string str)
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

/**
 * @param servers	list of server in which to search for the corresponding one
 * @param host		specified in the conf by the 'server_name' keyword
 * @param port		specified in the conf by the 'listen' keyword (ip_address:port)
 * @return			a pointer to the server corresponding to the couple (host, port) or the default server for this port if it exist, null overwise.
**/
Server	*select_server(const list<Server*> servers, string host, unsigned int port)
{
	DEBUG("Looking for " << host << ":" << port);
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
				if ((*server_name == "0.0.0.0" || *server_name == host) && (*server)->get_port() == port)
				{
					DEBUG("Found " << *server_name << ":" << (*server)->get_port());
					return (*server);
				}
			}
		}
	}
	if (default_server)
		DEBUG("Found default server for port: " << port);
	return default_server;
}

string 	send_socket(string message, string filepath, Server *server)
{
	if (filepath[filepath.length() - 1] == '/')
	{
		DEBUG("Target is a directory");
		filepath += server->get_index();
	}
	DEBUG(filepath);
	ifstream file(filepath.c_str(), ofstream::in);
	if (!file || !file.is_open() || !file.good() || file.fail() || file.bad())
	{
		message = code_404;
		file.close();
		file.open(server->get_root() + server->get_error_pages()[404], ofstream::in);
	}
	else if (message == "")
		message = code_200;
	string page((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	if (page == "") // if file is a directory or is empty
	{
		message = code_404;
		file.close();
		file.open(server->get_root() + server->get_error_pages()[404], ofstream::in);
		page = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	}
	file.close();
	string type = get_type(filepath);
	stringstream response;
	response << "HTTP/1.1 " << message << endl;
	response << "Date: " << get_time_stamp() << endl;
	response << "Server: webserv/0.01" << endl;
	response << "Content-Type: " << type << endl;
	response << "Content-Length: " << page.length() << endl;
	response << "Connection: Closed" << endl;
	response << endl;
	response << page;
	if (type == "text/html")
	{
		DEBUG("********* RESPONSE *********");
		DEBUG(response.str());
	}
	DEBUG("@@@@@@@@@@@@@@@@@@ END @@@@@@@@@@@@@@@@@@");
	return (response.str());
}

Location	*select_location(const Server *server, const string target)
{
	list<Location> locations = server->get_locations();
	if (locations.size() == 0)
		return NULL;
	//TODO: decompose filepath
	string searched_path = target;
	DEBUG("tmp: " << target);
	while (searched_path != "")
	{
		DEBUG(searched_path);
		for (list<Location>::iterator location = locations.begin(); location != locations.end(); location++)
		{
			DEBUG("searched_path: " << searched_path << "\t\t" << "path: " << (*location).get_path());
			if (searched_path == location->get_path())
			{
				DEBUG("Location found: " << location->get_path());
				return ((new Location(*location)));
			}
		}
		size_t pos = searched_path.find_last_of("/");
		if (pos == string::npos)
			break;
		searched_path = searched_path.substr(0, pos);
	}
	return NULL;
}

bool method_allowed(const Server *server, const string target, const string method)
{
	Location *location;
	if (!(location = select_location(server, target)))
		return false;
	list<string> HTTP_methods = location->get_HTTP_methods();
	for (list<string>::iterator HTTP_method = HTTP_methods.begin(); HTTP_method != HTTP_methods.end(); HTTP_method++)
	{
		if (*HTTP_method == method)
		{
			delete location;
			return true;
		}
	}
	delete location;
	return false;
}

string	Request::respond(const list<Server*> servers)
{
	Server *server = select_server(servers, headers["Host"], atoi(headers["Port"].c_str()));
	if (!server)
		return (send_socket(code_404, server->get_error_pages()[404], server));
	string filepath = string(server->get_root() + target);
	if (target.compare("/") == 0)
		filepath += server->get_index();
	if (!method_allowed(server, target, type))
		return (send_socket(code_405, server->get_error_pages()[405], server));
	if (type == "GET")
		return(send_socket("", filepath, server));
	else if (type == "POST")
	{

	}
	else if (type == "DELETE")
	{

	}
	return (send_socket(code_405, server->get_error_pages()[405], server));
	DEBUG("@@@@@@@@@@@@@@@@@@ END @@@@@@@@@@@@@@@@@@");
}
