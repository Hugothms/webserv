/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 16:29:23 by edal--ce          #+#    #+#             */
/*   Updated: 2021/09/08 15:02:02 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "request.hpp"

Request::Request(){}

Request::Request(char *buffer, int size, int sock) : socket(sock)
{
	int i = 0;
	while (i < size && buffer[i] && buffer[i] != ' ')
		type += buffer[i++];
	++i;
	while (i < size && buffer[i] && buffer[i] != ' ')
		target += buffer[i++];
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
	// if (DEBUG)
		// std::cout << "------ RESPONSE ------\n" << response.str() << std::endl;
	myfile.close();
}

Request::~Request() {}
