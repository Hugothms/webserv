/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 16:29:23 by edal--ce          #+#    #+#             */
/*   Updated: 2021/11/07 21:39:30 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "request.hpp"

// Request::Request(){}

Request::~Request() {}

Request::Request(const char *buffer, const size_t size, const int sock)
: socket(sock)//, content_length(0), port(0)
{
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
				host =get_str_before_char(request, "\n", &pos);
				port = "80";
			}
			if (host == "localhost")
				host = "127.0.0.1";
			headers.insert(pair<string, string>("Host", host));
			headers.insert(pair<string, string>("Port", port));
			continue;
		}
		headers.insert(pair<string, string>(header, get_str_before_char(request, "\n", &pos)));
	}
	pos++;
	if (headers.count("Content-Length"))
		headers.insert(pair<string, string>("Body", &request[pos]));

	DEBUG("\n****** REQUEST PARSED ******");
	DEBUG("type:" << type);
	DEBUG("target:" << target);
	DEBUG("socket:" << socket);
	DEBUG("----------------------------");
	map<string, string>::iterator it = headers.begin();
	while(it != headers.end())
		DEBUG(it->first << ": " << it++->second);
	DEBUG("****************************" << endl);
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

string 	send_socket(string message, string page)
{
	stringstream response;
	response << "HTTP/1.1 " << message << endl;
	response << "Date: " << get_time_stamp() << endl;
	response << "Server: my_httpd/1.0" << endl;
	response << "Content-Type: text/html" << endl;
	response << "Content-Length: " << page.length() << endl;
	response << endl;
	response << page;
	DEBUG("\n********* RESPONSE *********");
	DEBUG(response.str());
	return (response.str());
	// cl->set_response(&(response.str()));
	// send(socket, response.str().c_str(), response.str().length(), 0);
}

string	Request::respond(const list<Server*> servers)
{
	stringstream response;
	Server *server = select_server(servers, headers["Host"], atoi(headers["Port"].c_str()));
	if (!server)
		return (send_socket("404 Not Found", "<html><body><h1>404 Not Found</h1></body></html>"));
	string filepath(server->get_root());
	if (target.compare("/") == 0)
		target += server->get_index();
	filepath += target;
	if (type == "GET")
	{
		bool method_found = false;
		list<Location> locations = server->get_locations();
		for (list<Location>::iterator location = locations.begin(); location != locations.end(); location++)
		{
			list<string> HTTP_methods = location->get_HTTP_methods();
			for (list<string>::iterator HTTP_method = HTTP_methods.begin(); HTTP_method != HTTP_methods.end(); HTTP_method++)
				if (*HTTP_method == "GET")
					method_found = true;
		}
		if (!method_found)
			return (send_socket("405 Method Not Allowed", "<html><body><h1>405 Method Not Allowed</h1></body></html>"));
		ifstream myfile(filepath.c_str(), ofstream::in);
		if (!myfile)
		{
			myfile.close();
			string target = server->get_root() + server->get_error_pages()[404];
			myfile.open(target.c_str(), ofstream::in);
			response << "HTTP/1.1 404 Not Found" << endl;
		}
		else
			response << "HTTP/1.1 200 OK" << endl;
		string file((istreambuf_iterator<char>(myfile)), istreambuf_iterator<char>());
		response << "Server: webserv/0.01" << endl;
		response << "Date: " << get_time_stamp() << endl;
		response << "Content-Type: " << get_type(target) << endl;
		response << "Content-Length: " << file.length() << endl;
		response << "Connection: Cosed" << endl << endl;
		response << file;

		// send(socket, response.str().c_str(), response.str().length(), 0);
		// cl->set_response(&(response.str()));
		if (get_type(filepath) == "text/html")
		{
			DEBUG("\n********* RESPONSE *********");
			DEBUG(response.str());
		}
		myfile.close();
		return (response.str());
	}
	else if (type == "POST")
	{

	}
	else if (type == "DELETE")
	{

	}
	else
	{
		response << "HTTP/1.1 403 Forbidden" << endl;

	}
	return("");
}

string	Request::respond()
{
	string filepath("website");
	if (target.compare("/") == 0)
		target += "index.html";
	filepath += target;
	ifstream myfile(filepath.c_str(), ofstream::in);
	stringstream response;
	if (!myfile)
	{
		myfile.close();
		myfile.open("website/404.html", ofstream::in);
		response << "HTTP/1.1 404 Not Found" << endl;
	}
	else
		response << "HTTP/1.1 200 OK" << endl;
	string file((istreambuf_iterator<char>(myfile)),
                 istreambuf_iterator<char>());
	response << "Server: webserv/0.01" << endl;
	response << "Date: " << get_time_stamp() << endl;
	response << "Content-Type: " << get_type(target) << endl;
	response << "Content-Length: " << file.length() << endl;
	response << "Connection: Cosed" << endl;
	response << file;
	// cl->set_response(&(response.str()));
	// send(socket, response.str().c_str(), response.str().length(), 0);
	// DEBUG("\n********* RESPONSE *********");
	// DEBUG(response.str());
	myfile.close();
	return response.str();
}
