/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:47:21 by edal--ce          #+#    #+#             */
/*   Updated: 2021/09/08 16:58:17 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SERVER_HPP
#define SERVER_HPP

// #include "request.hpp"
#include "includes.hpp"

#define V4 AF_INET
#define V6 AF_INET6
#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM
#define IP 0
#define MAX_BACKLOG 10
#define MAX_CLIENTS 10

#define DEBUG 1

class Server
{
	private :
		class Client
  		{
  			private :

  			public :
  				int 	socket;
  				struct 	sockaddr_in addres;
  				char 	*message;
  				char 	*buffer;
  				int 	len;
  				size_t recv_bytes;
  				Client()
  				{
  					buffer = new char[4097];
  					recv_bytes = 0;
  					socket = -1;
  				}
  				~Client()
  				{
  					delete buffer;
  				}
  		};

		int 					_listen_sock;
		int 					_reuse;
		// int 					send_socket;
		unsigned int 			_port;
		struct sockaddr_in 		_addr;

		fd_set					_read_fds;
  		fd_set					_write_fds;
  		fd_set					_except_fds;
  		Client 					*_clients;//[MAX_CLIENTS];
  		
  		
  
		// std::vector<Request> 	requests;
		// struct sockaddr_in 	hint;
		// struct sockaddr_in addr;
		// unsigned int port;
 		// const char *hello = "HTTP/1.1 200 OK\nContent-Length: 52\nContent-Type: text/html\nConnection: Closed\n\n<html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>\n";
 	public:
		Server(unsigned int _port = 8080);
		~Server();
		int run();
		void s_listen();
		int setup();
		int setup_fd_set();
		int start_listen_socket();
		int handle_new_connection();
		// int receive_from_peer();
		int receive_from_peer(Client *peer);
		void process_packet(Client *peer);
};
#endif