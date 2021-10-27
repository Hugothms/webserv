/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 16:29:23 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/27 13:15:42 by hthomas          ###   ########.fr       */
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
			string host;
			string port;
			if ((host = get_str_before_char(request, ":", &pos)).length())
				port = get_str_before_char(request, "\n", &pos);
			else
			{
				host = get_str_before_char(request, "\n", &pos);
				port = "80";
			}
			// if (host == "localhost")
			// 	host = "127.0.0.1";
			headers.insert(pair<string, string>("Host", host));
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

//TODO : select server
Server	*Request::select_server(const list<Server*> servers, string host, unsigned int port)
{
	DEBUG("Looking for " << host << ":" << port);
	list<Server*>::const_iterator it = servers.begin();
	while (it != servers.end())
	{
		DEBUG((*it)->get_host() << ":" << (*it)->get_port());
		if (((*it)->get_host() == "0.0.0.0" || (*it)->get_host() == host) && (*it)->get_port() == port)
			return (*it);
		it++;
	}
	return NULL;
}

void 	send_socket(int socket, string message, string page, string type = "text/html")
{
	stringstream response;
	response << "HTTP/1.1 " << message << endl;
	response << "Date: " << get_time_stamp() << endl;
	response << "Server: webserv/0.01" << endl;
	response << "Content-Type: " << type << endl;
	response << "Content-Length: " << page.length() << endl;
	response << "Connection: Cosed" << endl;
	response << endl;
	response << page;
	if (type == "text/html")
	{
		DEBUG("********* RESPONSE *********");
		DEBUG(response.str());
	}
	send(socket, response.str().c_str(), response.str().length(), 0);
}

bool Request::method_allowed(Server *server, string method)
{
	list<Location> locations = server->get_locations();
	if (locations.size() == 0)
		return false;
	for (list<Location>::iterator location = locations.begin(); location != locations.end(); location++)
	{
		list<string> HTTP_methods = location->get_HTTP_methods();
		for (list<string>::iterator HTTP_method = HTTP_methods.begin(); HTTP_method != HTTP_methods.end(); HTTP_method++)
			if (*HTTP_method == method)
				return true;
	}
	return false;
}

void	Request::respond(const list<Server*> servers)
{
	Server *server = select_server(servers, headers["Host"], atoi(headers["Port"].c_str()));
	if (!server)
		return (send_socket(socket, "404 Not Found", "<html><body><h1>404 Not Found</h1></body></html>"));
	string message;
	string filepath(server->get_root());
	if (target.compare("/") == 0)
		target += server->get_index();
	filepath += target;
	if (type == "GET")
	{
		if (!method_allowed(server, "GET"))
			return (send_socket(socket, "405 Method Not Allowed", "<html><body><h1>405 Method Not Allowed</h1></body></html>"));
		ifstream file(filepath.c_str(), ofstream::in);
		if (!file)
		{
			file.close();
			file.open(server->get_root() + server->get_error_pages()[404], ofstream::in);
			message = "404 Not Found";
		}
		else
			message = "200 OK";
		string page((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
		send_socket(socket, message, page, get_type(filepath));
		file.close();
	}
	else if (type == "POST")
	{

	}
	else if (type == "DELETE")
	{

	}
	else
		return (send_socket(socket, "405 Method Not Allowed", "<html><body><h1>405 Method Not Allowed</h1></body></html>"));
	DEBUG("@@@@@@@@@@@@@@@@@@ END @@@@@@@@@@@@@@@@@@");
}
