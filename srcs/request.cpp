/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 16:29:23 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/05 17:09:10 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "request.hpp"

Request::Request(){}

std::string get_str_before_char(std::string str, std::string c, size_t *index)
{
	size_t length = str.find(c, *index) - *index;
	std::string res;
	if (length == std::string::npos || str.find('\n', *index) - *index < length)
		return res;
	res = str.substr(*index , length);
	*index += length + 1;
	return res;
}

Request::Request(char *buffer, size_t size, int sock) : socket(sock)//, content_length(0), port(0)
{
	size_t index = 0;
	std::string request(buffer, size);
	// std::string request = "GET / HTTP/1.1\nHost: localhost:8080\nUser-Agent: curl/7.64.1\nAccept: */*\n\r";
	type = get_str_before_char(request, " ", &index);
	target = get_str_before_char(request, " ", &index);
	get_str_before_char(request, "\n", &index);
	std::string header;
	while (index < size && request[index]) // headers parsing loop
	{
		header = get_str_before_char(request, ": ", &index);
		// DEBUG((int)header[0] << "/" << (int)header[1] << "\t|" << header << "|");
		if (header == "\0")
			break ; // case empty line
		index++;
		if (header == "Host")
		{
			headers.insert(std::pair<std::string, std::string>("Host", get_str_before_char(request, ":", &index)));
			headers.insert(std::pair<std::string, std::string>("Port", get_str_before_char(request, "\n", &index)));
			continue;
		}
		headers.insert(std::pair<std::string, std::string>(header, get_str_before_char(request, "\n", &index)));
	}
	index += 2;
	if (headers["Content-Length"].length())
		headers.insert(std::pair<std::string, std::string>("Body", &request[index]));


	DEBUG("\n******** PARSED ********");
	DEBUG("type:" << type);
	DEBUG("target:" << target);
	DEBUG("socket:" << socket);
	DEBUG("------------------------");
	std::map<std::string, std::string>::iterator it = headers.begin();
	while(it != headers.end())
		DEBUG(it->first << ": " << it++->second);
	DEBUG("");
}

std::string getdayofweek(int day)
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

std::string getmonth(int month)
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

std::string gettimestamp()
{
	time_t now = time(0);

	tm *time = gmtime(&now);
	std::stringstream output;
	output << getdayofweek(time->tm_wday);

	if (time->tm_mday < 10)
		output <<", 0" << time->tm_mday;
	else
		output <<", " << time->tm_mday;
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

std::string gettype(std::string str)
{
	std::string ret;
	if (str.find(".png", str.length() - 4) != std::string::npos)
		ret = "image/png";
	else if (str.find(".jpg", str.length() - 4) != std::string::npos)
		ret = "image/jpg";
	else if (str.find(".png", str.length() - 4) != std::string::npos)
		ret = "image/x-icon";
	else if (str.find(".css", str.length() - 4) != std::string::npos)
		ret = "text/css";
	else if (str.find(".js", str.length() - 3) != std::string::npos)
		ret = "application/javascript";
	else
		ret = "text/html";
	return ret;
}

void Request::respond()
{
	std::string filepath("website");
	if (target.compare("/") == 0)
		target += "index.html";
	filepath += target;
	std::ifstream myfile(filepath.c_str(), std::ofstream::in);
	std::stringstream response;
	if (!myfile)
	{
		myfile.close();
		myfile.open("website/404.html", std::ofstream::in);
		response << "HTTP/1.1 404 Not Found\n";
	}
	else
		response << "HTTP/1.1 200 OK\n";
	std::string file((std::istreambuf_iterator<char>(myfile)),
                 std::istreambuf_iterator<char>());
	response << "Server: webserv/0.01\n";
	response << "Date: " << gettimestamp();
	response << "Content-Type: " << gettype(target);
	response << "\nContent-Length: " << file.length();
	response << "\nConnection: Closed\n\n";
	response << file;
	send(socket, response.str().c_str(), response.str().length(), 0);
	// DEBUG("------- RESPONSE -------");
	// DEBUG(response.str());
	myfile.close();
}

Request::~Request() {}
