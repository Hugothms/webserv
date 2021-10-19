/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 16:29:23 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/19 17:09:03 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "request.hpp"

// Request::Request(){}

Request::~Request() {}

Request::Request(char *buffer, size_t size, int sock) : socket(sock)//, content_length(0), port(0)
{
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
			headers.insert(pair<string, string>("Host", get_str_before_char(request, ":", &pos)));
			headers.insert(pair<string, string>("Port", get_str_before_char(request, "\n", &pos)));
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
	DEBUG("****************************\n");
}

string getdayofweek(int day)
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

string getmonth(int month)
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

string gettimestamp()
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
	output << time->tm_sec << " GMT\n";
	return (output.str());
}

string gettype(string str)
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

// void Request::respond(Server server)
// {
// 	if (type == "GET")
// 	{
// 		if (target == "/")
// 			target = "/index.html";
// 		if (target == "/favicon.ico")
// 			target = "";
// 		string filename = "./" + server.get_root() + target;
// 		string type = gettype(filename);
// 		DEBUG("filename:" << filename);
// 		DEBUG("type:" << type);
// 		ifstream file(filename, ios::in | ios::binary);
// 		if (file)
// 		{
// 			file.seekg(0, file.end);
// 			int length = file.tellg();
// 			file.seekg(0, file.beg);
// 			char *data = new char[length];
// 			file.read(data, length);
// 			file.close();
// 			string response = "HTTP/1.1 200 OK\nDate: " + gettimestamp() + "\nContent-Type: " + type + "\nContent-Length: " + to_string(length) + "\n\n";
// 			send(socket, response.c_str(), response.length(), 0);
// 			send(socket, data, length, 0);
// 			delete[] data;
// 		}
// 		else
// 		{
// 			string response = "HTTP/1.1 404 Not Found\nDate: " + gettimestamp() + "\nContent-Type: text/plain\nContent-Length: 13\n\n404 Not Found";
// 			send(socket, response.c_str(), response.length(), 0);
// 		}
// 	}
// }

Server *Request::select_server(list<Server*> servers)
{
	return servers.front();
}

void Request::respond(list<Server*> servers)
{
	Server *server = select_server(servers);
	string filepath(server->get_root());
	if (target.compare("/") == 0)
		target += server->get_index();
	filepath += target;
	ifstream myfile(filepath.c_str(), ofstream::in);
	stringstream response;
	if (!myfile)
	{
		myfile.close();
		string target = server->get_root() + server->get_error_pages()[404];
		myfile.open(target, ofstream::in);
		response << "HTTP/1.1 404 Not Found\n";
	}
	else
		response << "HTTP/1.1 200 OK\n";
	string file((istreambuf_iterator<char>(myfile)),
                 istreambuf_iterator<char>());
	response << "Server: webserv/0.01\n";
	response << "Date: " << gettimestamp();
	response << "Content-Type: " << gettype(target);
	response << "\nContent-Length: " << file.length();
	response << "\nConnection: Closed\n\n";
	response << file;
	send(socket, response.str().c_str(), response.str().length(), 0);
	DEBUG(gettype(filepath));
	if (gettype(filepath) == "text/html")
	{
		DEBUG("********* RESPONSE *********");
		DEBUG(response.str());
	}
	myfile.close();
}

void Request::respond()
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
		response << "HTTP/1.1 404 Not Found\n";
	}
	else
		response << "HTTP/1.1 200 OK\n";
	string file((istreambuf_iterator<char>(myfile)),
                 istreambuf_iterator<char>());
	response << "Server: webserv/0.01\n";
	response << "Date: " << gettimestamp();
	response << "Content-Type: " << gettype(target);
	response << "\nContent-Length: " << file.length();
	response << "\nConnection: Closed\n\n";
	response << file;
	send(socket, response.str().c_str(), response.str().length(), 0);
	// DEBUG("********* RESPONSE *********");
	// DEBUG(response.str());
	myfile.close();
}
