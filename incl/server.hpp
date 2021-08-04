/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:47:21 by edal--ce          #+#    #+#             */
/*   Updated: 2021/08/04 19:52:48 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SERVER_HPP
#define SERVER_HPP

#include "request.hpp"

#include <vector>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 

#define V4 AF_INET
#define V6 AF_INET6
#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM
#define IP 0

class Server
{
	private :
		int 					listen_socket;
		int 					send_socket;
		unsigned int 			port;
		struct sockaddr_in 		hint;
		std::vector<Request> 	requests;
		// struct sockaddr_in 	hint;
		// struct sockaddr_in addr;
		// unsigned int port;
 		// const char *hello = "HTTP/1.1 200 OK\nContent-Length: 52\nContent-Type: text/html\nConnection: Closed\n\n<html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>\n";
 	public:
		Server(unsigned int _port = 8080);
		~Server();
		void s_listen();
};
#endif