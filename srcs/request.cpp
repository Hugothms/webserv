/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 16:29:23 by edal--ce          #+#    #+#             */
/*   Updated: 2021/09/20 15:29:16 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "request.hpp"

Request::Request(){}

std::string get_str_before_char(std::string str, std::string c, size_t *index)
{
	size_t length = str.find(c, *index) - *index;
	std::string res;
	if (length == std::string::npos)
	{
		DEBUG("\"" << c << "\" NOT FOUND IN STRING");
		return res;
	}
	res = str.substr(*index , length);
	*index += length + 1;
	return res;
}

Request::Request(char *buffer, size_t size, int sock) : socket(sock)
{
	size_t index = 0;
	std::string request(buffer, size);
	type = get_str_before_char(request, " ", &index);
	target = get_str_before_char(request, " ", &index);
	get_str_before_char(request, "\n", &index);
	std::string header;
	while (index < size && request[index]) // headers parsing loop
	{
		header = get_str_before_char(request, ": ", &index);
		if (header[1] == '\n')
			break ; // case header is empty
		index++;
		if (header == "Host")
		{
			host = get_str_before_char(request, ":", &index);
			socket = atoi(get_str_before_char(request, "\n", &index).c_str());
		}
		else if (header == "User-Agent")
			user_agent = get_str_before_char(request, "\n", &index);
		else if (header == "Accept")
			accept = get_str_before_char(request, "\n", &index);
		else if (header == "Accept-Language")
			accept_language = get_str_before_char(request, "\n", &index);
		else if (header == "Accept-Encoding")
			accept_encoding = get_str_before_char(request, "\n", &index);
		else if (header == "Connection")
			connection = get_str_before_char(request, "\n", &index);
		else if (header == "Upgrade-Insecure-Requests")
			upgrade_insecure_requests = get_str_before_char(request, "\n", &index);
		else if (header == "Sec-Fetch-Dest")
			sec_fetch_dest = get_str_before_char(request, "\n", &index);
		else if (header == "Sec-Fetch-Mode")
			sec_fetch_mode = get_str_before_char(request, "\n", &index);
		else if (header == "Sec-Fetch-Site")
			sec_fetch_site = get_str_before_char(request, "\n", &index);
		else if (header == "Sec-Fetch-User")
			sec_fetch_user = get_str_before_char(request, "\n", &index);
		else if (header == "Sec-GPC")
			sec_gpc = get_str_before_char(request, "\n", &index);
		else if (header == "Cache-Control")
			cache_control = get_str_before_char(request, "\n", &index);
		else // ignore/skip unkonwn headers
			std::string trash = get_str_before_char(request, "\n", &index);
		// todo: Continue parsing of header here
	}
	// todo: Continue parsing of body here


	DEBUG("\n******** PARSED ********");
	DEBUG("type:" << type);
	DEBUG("target:" << target);
	DEBUG("host:" << host);
	DEBUG("accept:" << accept);
	DEBUG("accept_language:" << accept_language);
	DEBUG("accept_encoding:" << accept_encoding);
	DEBUG("dnt:" << dnt);
	DEBUG("connection:" << connection);
	DEBUG("upgrade_insecure_requests:" << upgrade_insecure_requests);
	DEBUG("sec_fetch_dest:" << sec_fetch_dest);
	DEBUG("sec_fetch_mode:" << sec_fetch_mode);
	DEBUG("sec_fetch_site:" << sec_fetch_site);
	DEBUG("sec_fetch_user:" << sec_fetch_user);
	DEBUG("sec_gpc:" << sec_gpc);
	DEBUG("cache_control:" << cache_control);
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
	// DEBUG("------- RESPONSE -------\n");
	// DEBUG(response.str());
	myfile.close();
}

Request::~Request() {}
