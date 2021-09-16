/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:47:21 by edal--ce          #+#    #+#             */
/*   Updated: 2021/09/16 14:40:35 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.hpp"
#include "request.hpp"
#include "includes.hpp"

#define BUFFER_SIZE 1024

#define V4 AF_INET
#define V6 AF_INET6
#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM
#define IP 0

class Server
{
	private :
		int						listen_socket;
		std::string				server_name; //Host header in HTTP request
		int						send_socket;
		unsigned int			port;
		struct sockaddr_in		hint;
		std::vector<Request>	requests;

	public:
		Server(unsigned int _port = 8080);
		~Server();
		void s_listen();
};

#endif
