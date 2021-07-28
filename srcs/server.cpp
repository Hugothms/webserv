/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2021/07/28 16:20:40 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "server.hpp"

Server::~Server() {}
Server::Server()
{
	// port = 8080;

	
	
	int opt = 1;
	//Creating a socket on IPv4 mode, using TCP/IP protocol
	if ((listen_socket = socket(V4, TCP, IP)) == 0)
	{
		std::cerr << "socket init failed !\n";
		exit(1);
	}
	if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR /*| SO_REUSEPORT*/, &opt, sizeof(opt)))
	{
		std::cerr << "setsockopt failed !\n";
		exit(1);	
	}
	//IPV4 mode, use sockaddr_in6 for V6
	hint.sin_family = V4;
	
	//Bind to any IP ont the machine
	hint.sin_addr.s_addr = INADDR_ANY;
	//We have to change big endian to little endian, so change 8080
	//HTONS = Host To Network Short | NTOHS is the reverse
	hint.sin_port = htons(8080);
	
	//Bind it ?
	if (bind(listen_socket, ( struct sockaddr*) &hint, sizeof(hint)) < 0)
	{
		std::cerr << "socket bind failed !\n";
		exit(1);
	}
	//Set it to listen
	if (listen(listen_socket, SOMAXCONN) == -1)
	{
		std::cerr << "Can't listen !\n";
		exit(1);
	}
}

void Server::s_listen()
{
	struct sockaddr_in client;
	socklen_t client_size = sizeof(client);
	send_socket = accept(listen_socket, 
		(struct sockaddr*) &client, &client_size);
	
	if (send_socket == -1)
	{
		std::cerr << "Can't client sock\n";
		exit(-1);
	}
	char buffer[1024] ;
	int query_size = read(send_socket, buffer, 1024);

	write(1, buffer, query_size);
	send(send_socket, hello, strlen(hello), 0);
	close(send_socket);

	
}