/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 16:29:23 by edal--ce          #+#    #+#             */
/*   Updated: 2021/08/04 12:54:43 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "request.hpp"

Request::Request(){}

Request::Request(char *buffer, int size, int sock) : socket(sock)
{
	int i = 0;
	while (i < size && buffer[i] && buffer[i] != ' ')
	{
		type += buffer[i++];
	}
	++i;
	// std::cout << "Request type is " << type << std::endl;
	while (i < size && buffer[i] && buffer[i] != ' ')
	{
		target += buffer[i++];
	}
	// std::cout << "Target is " << target << std::endl;
	
}

void Request::respond()
{
	std::string filepath("website");
	if (target.compare("/") == 0)
		target += "index.html";
	
	filepath += target;
	// std::cout << "The path is "<< filepath << std::endl;
	std::ifstream myfile(filepath,std::ofstream::in);
	// std::string response;
	if (myfile)
	{
		std::cout << "Found file\n";

	}
	else
	{
		std::cout << "Did not\n";
		myfile.close();
		myfile.open("website/404.html", std::ofstream::in);
		if (myfile == 0)
			std::cout << "OHNO\n";
	}
	std::stringstream response;

	std::string file((std::istreambuf_iterator<char>(myfile)),
                 std::istreambuf_iterator<char>());
	response << "HTTP/1.1 200 OK\n";
	response << "Content-Length: ";
	response << file.length();
	response << "\n";
	response << "Content-Type: text/html; charset=UTF-8\n";
	response << "Connection: Closed\n\n";

	response << file;
	// send(socket, hello, strlen(hello), 0);

	send(socket, response.str().c_str(), response.str().length(), 0);
	std::cout << "SENDING " << response.str() << std::endl;
	myfile.close();
}

Request::~Request() {}
