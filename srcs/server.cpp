/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/11 14:42:05 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::~Server() {}
Server::Server(	std::list<Location>			locations,
				std::list<std::string>		server_names,
				std::list<std::string>		error_pages,
				unsigned int 				port,
				std::string					root,
				std::string					index,
				unsigned int				max_client_body_size)
	: locations(locations), server_names(server_names), error_pages(error_pages), port(port), root(root), index(index), max_client_body_size(max_client_body_size)
{
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
		exit(2);
	}
	//IPV4 mode, use sockaddr_in6 for V6
	hint.sin_family = V4;
	//Bind to any IP ont the machine
	hint.sin_addr.s_addr = INADDR_ANY;
	//We have to change big endian to little endian, so change 8080
	//HTONS = Host To Network Short | NTOHS is the reverse
	hint.sin_port = htons(port);

	//Bind it ?
	if (bind(listen_socket, ( struct sockaddr*) &hint, sizeof(hint)) < 0)
	{
		std::cerr << "socket bind failed !\n";
		exit(3);
	}
	//Set it to listen
	if (listen(listen_socket, SOMAXCONN) == -1)
	{
		std::cerr << "Can't listen !\n";
		exit(4);
	}
}
//https://jvns.ca/blog/2017/06/03/async-io-on-linux--select--poll--and-epoll/

//Need to use kqueue vs epoll
void Server::s_listen()
{
	struct sockaddr_in client;
	socklen_t client_size = sizeof(client);
	//Accept functions give you a new file descriptor
	send_socket = accept(listen_socket, (struct sockaddr*) &client, &client_size);
	//Need to use either Poll Epoll or Select
	if (send_socket == -1)
	{
		std::cerr << "Can't client sock\n";
		exit(-1);
	}


	char buffer[BUFFER_SIZE];
	int query_size = read(send_socket, buffer, BUFFER_SIZE);
	buffer[query_size] = '\0';
	DEBUG("-------- REQUEST --------");
	DEBUG(buffer);
	Request query(buffer, query_size, send_socket);
	query.respond();
	close(send_socket);
}
