/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:47:21 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/07 14:43:00 by hthomas          ###   ########.fr       */
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
		// * PARSED FROM CONFIG FILE *
		unsigned int				port; // port to listen and send on ("listen" in config file)
		std::vector<std::string>	server_names; // "Host" header in HTTP request (domain names)
		std::string					root; // directory where the webste is
		std::string					index; // file served when "/" is requested
		std::vector<std::string>	error_pages; //
		unsigned int				max_client_body_size;

		// * FOR "INTERNAL" USE *
		int							listen_socket; // created by socket
		int							send_socket; // ?
		struct sockaddr_in			hint; // ?
		std::vector<Request>		requests;

	public:
		Server(	std::vector<std::string>	server_names,
				std::vector<std::string>	error_pages,
				unsigned int				_port = 8080,
				std::string					root = "website",
				std::string					index = "index.html",
				unsigned int				max_client_body_size = 2048);
		~Server();
		void s_listen();
};

#endif
