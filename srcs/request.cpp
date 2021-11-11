/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 16:29:23 by edal--ce          #+#    #+#             */
/*   Updated: 2021/11/11 13:14:54 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
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

void 	get_body(const Server *server, string &message, string filepath, string &body)
{
	ifstream file(filepath.c_str(), ofstream::in);
	if (!file || !file.is_open() || !file.good() || file.fail() || file.bad())
	{
		message = code_404;
		file.close();
		file.open(error_page(server, 404), ofstream::in);
	}
	else if (message == "")
		message = code_200;
	body = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	if (body == "") // if file is a directory or is empty
	{
		message = code_404;
		file.close();
		file.open(error_page(server, 404), ofstream::in);
		body = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	}
	file.close();
}

string 	get_response(const Server *server, string message, string filepath)
{
	string response;
	string body;
	get_body(server, message, filepath, body);
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
	//TODO: decompose filepath
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
	// DEBUG("filepath: " << filepath);
	// DEBUG("target: " << target);
	// DEBUG("path(): " << location->get_path());
	// DEBUG("root(): " << location->get_location_root());
	// DEBUG("pos:" << pos);
	// DEBUG("");
	if (pos != string::npos)
	{
		string tmp = target.substr(pos + location->get_path().length());
		target.resize(pos);
		filepath += '/' + location->get_location_root() + '/' + tmp;
	}
	else
		filepath += target;
	if (filepath[filepath.length() - 1] == '/')
	{
		// DEBUG("Target is a directory");
		if (location->get_index().length())
			filepath += location->get_index();
		else
			filepath += server->get_index();
	}
	// DEBUG("filepath: " << filepath << endl << endl);
}

string	Request::respond(const list<Server*> &servers)
{
	server = select_server(servers, headers["Host"], atoi(headers["Port"].c_str()));
	if (!server)
		return (get_response(server, code_404, error_page(server, 404)));
	location = select_location(server, target);
	if (!location)
		return (get_response(server, code_404, error_page(server, 404)));
	if (!method_allowed(location, type))
		return (get_response(server, code_405, error_page(server, 405)));
	set_filepath();
	string message;
	if (type == "GET")
		return(get_response(server, "", filepath));
	else if (type == "HEAD")
	{
		string body;
		get_body(server, message, filepath, body);
		return (get_header(message, get_type(filepath), body.length()));
	}
	else if (type == "POST")
	{
		mkdir((server->get_root() + '/' + location->get_upload_directory()).c_str(), S_IRWXU|S_IRWXG|S_IRWXO);
		filepath = server->get_root() + '/' + location->get_upload_directory() + target;
		// ofstream file;
		// file.open(filepath);
		ofstream file_out(filepath, ios::app);
		file_out << headers["Body"] << endl;
		file_out.close();
		return (get_response(server, "", filepath));
	}
	else if (type == "DELETE")
	{
		//todo
		filepath = server->get_root() + '/' + location->get_upload_directory() + target;
		remove(filepath.c_str());
		return (get_response(server, "", filepath));
	}
	return (get_response(server, code_405, error_page(server, 405)));
}
