/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:47:21 by edal--ce          #+#    #+#             */
/*   Updated: 2021/09/16 11:17:07 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.hpp"
#include "request.hpp"
#include "includes.hpp"

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

 	public:
		Server(unsigned int _port = 8080);
		~Server();
		void s_listen();
};

#endif
