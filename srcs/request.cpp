/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 16:29:23 by edal--ce          #+#    #+#             */
/*   Updated: 2021/08/03 16:33:04 by edal--ce         ###   ########.fr       */
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
	std::cout << "Request type is " << type << std::endl;
	while (i < size && buffer[i] && buffer[i] != ' ')
	{
		target += buffer[i++];
	}
	std::cout << "Target is " << target << std::endl;
	
}

void Request::respond()
{
	std::string filepath("website");
	if (target.compare("/") == 0)
		target += "index.html";
	
	filepath += target;
	std::cout << "The path is "<< filepath << std::endl;
	std::ifstream myfile(filepath,std::ofstream::in);
	if (myfile)
	{
		std::cout << "Found file\n";
	}
	else
	{
		std::cout << "Did not\n";
		myfile.open("website/404.hml");
	}
	std::string response((std::istreambuf_iterator<char>(myfile)),
                 std::istreambuf_iterator<char>());
	 send(socket, response.c_str(), response.length(), 0);
	 myfile.close();
}

Request::~Request() {}
