/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2021/09/20 16:44:46 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "server.hpp"

// #include <sys/socket.h>

#include <fcntl.h>

#define NO_SOCKET -1
#define BACKLOG 5
#define MAX_SEND_SIZE 10000
Server::~Server() 
{
	delete _clients;
}
Server::Server(unsigned int port) : _port(port)
{
	// memset(&_addr, 0, sizeof(_addr));
	// _clients = new Client[10];
	// _addr.sin_family = V4;
	// _addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// _addr.sin_port = htons(_port);
	// start_listen_socket();
}

int Server::setup_server()
{
	int client_socket, addr_size;
	struct sockaddr_in server_addr;

	int server_sock = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);

	if (bind(server_sock,(const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
		std::cerr << "BIND ERROR\n";
	if (listen(server_sock, BACKLOG) == -1)
		std::cerr << "LISTEN ERROR\n";
	return server_sock;
}

int Server::setup()
{
	server_socket = setup_server();
	std::cerr << "Now accepting connections on " << _port << std::endl;
	// if (start_listen_socket())
	// 	return -1;
	// if (setup_fd_set())
	return 0;
}

#define BUFFER_SIZE 2048

int Server::handle_new_connection()
{
	std::cout << "HANDLE_CONN\n";
	char buffer[BUFFER_SIZE];
	size_t bytes_read;
	int msg_size = 0;

	int new_client_sock = accept(_listen_sock, (struct sockaddr *)&client_addr, &client_len);
	// read()
	// struct sockaddr_in client_addr;
	// memset(&client_addr, 0, sizeof(client_addr));
	// socklen_t client_len = sizeof(client_addr);

	// int new_client_sock = accept(_listen_sock, (struct sockaddr *)&client_addr, &client_len);
	// if (new_client_sock < 0) 
	// {
	// 	perror("accept()");
	// 	return -1;
	// }
	
	// // if (DEB)
	// char client_ipv4_str[INET_ADDRSTRLEN];
	// inet_ntop(AF_INET, &client_addr.sin_addr, client_ipv4_str, INET_ADDRSTRLEN);
	
	// printf("Incoming connection from %s:%d.\n", client_ipv4_str, client_addr.sin_port);
	
	
	// for (int i = 0; i < MAX_CLIENTS; ++i) 
	// {
	// 		if (_clients[i].socket == -1) 
	// 		{
	// 			_clients[i].socket = new_client_sock;
	// 			_clients[i].addres = client_addr;
	// 			fcntl(_clients[i].socket, F_SETFL, O_NONBLOCK);
	// 			// fnctl(_clients[i].socket, F_SETFL, O_NONBLOCK);
	// 			// _clients[i].current_sending_byte   = -1;
	// 			// _clients[i].current_receiving_byte = 0;
				return 0;
	// 		}
	// }
	
	// printf("There is too much connections. Close new connection %s:%d.\n", client_ipv4_str, client_addr.sin_port);
	// close(new_client_sock);
	// return -1;
}

// void Server::process_packet(Client *client)
// {
// 		// printf("Message: %s: \n", buff);
// }

// void Server::close_client_connection(Client *client)
// {
// 	// close(client->socket);
// 	// client->socket = -1;
// 	// dequeue_all(&client->send_buffer);
// 	// client->current_sending_byte   = -1;
// 	// client->current_receiving_byte = 0;
// }

// int Server::send_to_peer(Client* client)
// {
// 	// write(1, client->buffer, client->recv_bytes);
// 	return 0;
// }

// void memcpy(char *src, char *dst, int size)
// {
// 	// for (int i = 0; i < size; i++)
// 	// {
// 	// 	dst[i] = src[i];
// 	// }
// }

// int Server::receive_from_peer(Client *peer)//, int (*message_handler)(Client *))
// {
// 	// std::cout << "TRYING TO RECEIVE : \n";
// 	// char buffer[1000];
// 	// int i = 0;
// 	// while ((peer->recv_bytes = recv(peer->socket, buffer, 1000, 0)) > 0)
// 	// {
// 	// 	// ++(peer->recv_bytes);
// 	// 	write(1, peer->buffer, peer->recv_bytes);
// 	// 	std::cout << peer->recv_bytes;
// 	// }
// 	// std::cout << "\nRECEIVE DONE : ";
// 	// std::cout << peer->recv_bytes << std::endl;
// 	// return 0;
// }




int Server::run()
{
	fd_set current_sockets, ready_sockets; 

	// server_socket = 
	FD_ZERO(&current_sockets);
	FD_SET(server_socket, &current_sockets);
	while (true)
	{
		// std::cout << "Waiting for connections...\n";
		//Temp copy
		ready_sockets = current_sockets;
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
			std::cerr << "Select fail\n";

		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &ready_sockets))
			{
				//New connection incomming
				if (i == server_socket)
				{
					int client_socket = handle_new_connection();
					FD_SET(client_socket, &current_sockets) ;
				}
				else //Read the data and handle
				{
					// handle_connection(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}
	}
	// int high_sock;// = _listen_sock;
	// while (1)
	// {
	// 	setup_fd_set();
	// 	high_sock = _listen_sock;

	// 	for (int i = 0; i < MAX_CLIENTS; i++) 
	// 	{
	// 		if (_clients[i].socket > high_sock)
	// 			high_sock = _clients[i].socket;
	// 	}
	// 	//LIST SOCKETS WE WANT TO CHECK
	// 	// int activity = select(high_sock + 1, &_read_fds, NULL, NULL , NULL);
	// 	if (select(high_sock + 1, &_read_fds, NULL, NULL , NULL) < 0)
	// 		std::cerr << "Oh shit\n";
	// 	for (int i = 0; i < MAX_CLIENTS; i++)
	// 	{
	// 		if (FD_ISSET(i, high_sock))
	// 	}
	// 	// int activity = select(high_sock + 1, &_read_fds, &_write_fds, &_except_fds, NULL);
		
	// }
	return 0;
}

int Server::start_listen_socket()	//Prepares socket, CRITICAL
{
	
 	return 0;
}

int Server::setup_fd_set()
{
// 	FD_ZERO(&_read_fds);
// 	FD_SET(_listen_sock, &_read_fds);

	return 0;
}

// void handle_signal_action(int sig_number)
// {
//   if (sig_number == SIGINT) {
//     printf("SIGINT was catched!\n");
//     shutdown_properly(EXIT_SUCCESS);
//   }
//   else if (sig_number == SIGPIPE) {
//     printf("SIGPIPE was catched!\n");
//     shutdown_properly(EXIT_SUCCESS);
//   }
// }
// Server::Server(unsigned int _port) : port(_port)
// {
// 	int opt = 1;

// 	//Creating a socket on IPv4 mode, using TCP/IP protocol
// 	if ((_listen_sock = socket(V4, TCP, IP)) == 0)
// 	{
// 		std::cerr << "socket init failed !\n";
// 		exit(1);
// 	}
// 	if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR /*| SO_REUSEPORT*/, &opt, sizeof(opt)))
// 	{
// 		std::cerr << "setsockopt failed !\n";
// 		exit(2);	
// 	}
// 	//IPV4 mode, use sockaddr_in6 for V6
// 	hint.sin_family = V4;
// 	//Bind to any IP ont the machine
// 	hint.sin_addr.s_addr = INADDR_ANY;
// 	//We have to change big endian to little endian, so change 8080
// 	//HTONS = Host To Network Short | NTOHS is the reverse
// 	hint.sin_port = htons(port);
	
// 	//Bind it ?
// 	if (bind(listen_sock, ( struct sockaddr*) &hint, sizeof(hint)) < 0)
// 	{
// 		std::cerr << "socket bind failed !\n";
// 		exit(3);
// 	}
// 	//Set it to listen
// 	if (listen(listen_sock, SOMAXCONN) == -1)
// 	{
// 		std::cerr << "Can't listen !\n";
// 		exit(4);
// 	}
// }
//https://jvns.ca/blog/2017/06/03/async-io-on-linux--select--poll--and-epoll/

//Need to use kqueue vs epoll
// int Server::setup_signals()
// {
//   struct sigaction sa;
//   sa.sa_handler = handle_signal_action;
//   if (sigaction(SIGINT, &sa, 0) != 0) {
//     perror("sigaction()");
//     return -1;
//   }
//   if (sigaction(SIGPIPE, &sa, 0) != 0) {
//     perror("sigaction()");
//     return -1;
//   }
  
//   return 0;
// }
// void Server::s_listen()
// {
// 	struct sockaddr_in client;
// 	socklen_t client_size = sizeof(client);
// 	//Accept functions give you a new file descriptor
// 	send_socket = accept(listen_sock, (struct sockaddr*) &client, &client_size);
// 	//Need to use either Poll Epoll or Select 
// 	if (send_socket == -1)
// 	{
// 		std::cerr << "Can't client sock\n";
// 		exit(-1);
// 	}
	

// 	char buffer[1024] ;

// 	int query_size = read(send_socket, buffer, 1024);
// 	Request query(buffer, 1024, send_socket); 
// 	write(1, buffer, query_size);
// 	query.respond();
// 	close(send_socket);	
// }