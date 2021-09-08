/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2021/09/09 01:45:38 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "server.hpp"

#define NO_SOCKET -1

#define MAX_SEND_SIZE 10000
Server::~Server() 
{
	delete _clients;
}
Server::Server(unsigned int port) : _port(port)
{
	memset(&_addr, 0, sizeof(_addr));
	_clients = new Client[10];
	_addr.sin_family = V4;
	_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	_addr.sin_port = htons(_port);
	
	// start_listen_socket();
}

int Server::setup()
{
	if (start_listen_socket())
		return -1;
	if (setup_fd_set())
		return -1;
}

int Server::handle_new_connection()
{
	struct sockaddr_in client_addr;
	memset(&client_addr, 0, sizeof(client_addr));
	socklen_t client_len = sizeof(client_addr);

	int new_client_sock = accept(_listen_sock, (struct sockaddr *)&client_addr, &client_len);
	if (new_client_sock < 0) 
	{
		perror("accept()");
		return -1;
	}
	
	// if (DEB)
	char client_ipv4_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &client_addr.sin_addr, client_ipv4_str, INET_ADDRSTRLEN);
	
	printf("Incoming connection from %s:%d.\n", client_ipv4_str, client_addr.sin_port);
	
	
	for (int i = 0; i < MAX_CLIENTS; ++i) 
	{
			if (_clients[i].socket == -1) 
			{
				_clients[i].socket = new_client_sock;
				_clients[i].addres = client_addr;
				// _clients[i].current_sending_byte   = -1;
				// _clients[i].current_receiving_byte = 0;
				return 0;
			}
	}
	
	printf("There is too much connections. Close new connection %s:%d.\n", client_ipv4_str, client_addr.sin_port);
	close(new_client_sock);
	return -1;
}

void Server::process_packet(Client *client)
{
		// printf("Message: %s: \n", buff);
}

void Server::close_client_connection(Client *client)
{
	close(client->socket);
	client->socket = -1;
	// dequeue_all(&client->send_buffer);
	// client->current_sending_byte   = -1;
	// client->current_receiving_byte = 0;
}

int Server::send_to_peer(Client* client)
{

}

int Server::receive_from_peer(Client *peer)//, int (*message_handler)(Client *))
{

  	printf("Ready for recv() from \n");
  
  	size_t len_to_receive = 0;
  	size_t received_count = 0;
  	size_t received_total = 0;

  	char *rec_buff;
  	do 
  	{
    // Is completely received?
    	if (peer->recv_bytes >= sizeof(peer->buffer)) 
    	{
    		write(2, "NOW HERE\n", 9);
      	process_packet(peer);
      	printf("Received message from client.\n");
      	// close_client_connection(peer);
      	// process_packet(&peer->receiving_buffer);
      	peer->recv_bytes = 0;
    	}
    	else
    	{}
    
//     // // Count bytes to send.
    	// len_to_receive = sizeof(peer->buffer) - peer->recv_bytes;
    	len_to_receive = 4097;
    	// if (len_to_receive > MAX_SEND_SIZE)
     //  	len_to_receive = MAX_SEND_SIZE;
    
    printf("Let's try to recv() %zd bytes... ", len_to_receive);
    memset(peer->buffer, 0, 4097);
    received_count = recv(peer->socket, (char *)(peer->buffer + peer->recv_bytes), len_to_receive, MSG_DONTWAIT);
    write(2,"RECEIVED: ", 10);
    write(2,peer->buffer + peer->recv_bytes , len_to_receive);
    if (received_count < 0) 
    {
				printf("recv() from peer error\n");
				return -1;
      // }
    }
    else if (received_count == 0)
    {
			printf("recv() 0 bytes. Peer gracefully shutdown.\n");
    }
    else if (received_count > 0)
    {
    	  peer->recv_bytes += received_count;
      	received_total += received_count;
      	write(2, peer->buffer, received_count);
      	printf("recv() %zd bytes\n", received_count);
    }
    // else if (received_count < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    //   break;
    // }
//     // // If recv() returns 0, it means that peer gracefully shutdown. Shutdown client.
    // if (received_count == 0) {
//     //   printf("recv() 0 bytes. Peer gracefully shutdown.\n");
//     //   return -1;
//     // }
//     // else if (received_count > 0) {
//     //   peer->current_receiving_byte += received_count;
//     //   received_total += received_count;
//     //   printf("recv() %zd bytes\n", received_count);
//     // }
  } while (received_count > 0);
  
  printf("Total recv()'ed %zu bytes.\n", received_total);
  return 0;
}

int Server::run()
{
	int high_sock;// = _listen_sock;
	while (1)
	{
		setup_fd_set();
		high_sock = _listen_sock;

		for (int i = 0; i < MAX_CLIENTS; i++) 
		{
			if (_clients[i].socket > high_sock)
				high_sock = _clients[i].socket;
		}
		int activity = select(high_sock + 1, &_read_fds, &_write_fds, &_except_fds, NULL);
		
		switch(activity)
		{
			case -1 :
				std::cerr << "-1 ACTIVITY\n";
				return -1;
			case 0 :
				std::cerr << "0 ACTIVITY\n";
				return -1;
			default :
				if (FD_ISSET(_listen_sock, &_read_fds)) 
				{
					handle_new_connection();
				}
				if (FD_ISSET(_listen_sock, &_except_fds)) 
				{
					std::cerr << "Exception listen socket fd.\n";
					return -1;
					//shutdown_properly(EXIT_FAILURE);
				}
				for (int i = 0; i < MAX_CLIENTS; i++) 
				{
					if (_clients[i].socket != NO_SOCKET && FD_ISSET(_clients[i].socket, &_read_fds)) 
					{
						if (receive_from_peer(&(_clients[i])) != 0) 
						{
							std::cout << "DONE RCV\n" ;
							close_client_connection(&(_clients[i]));
							// close_client_connection(&_clients[i]);
							continue;
						}
					}
					if (_clients[i].socket != NO_SOCKET && FD_ISSET(_clients[i].socket, &_write_fds)) 
					{
						if (send_to_peer(&_clients[i]) != 0) 
						{
							close_client_connection(&_clients[i]);
							continue;
						}
					}
					// if (_clients[i].socket != NO_SOCKET && FD_ISSET(_clients[i].socket, &_write_fds)) 
					// {
					// 	if (send_to_peer(&_clients[i]) != 0) 
					// 	{
					// 		close_client_connection(&_clients[i]);
					// 		continue;
					// 	}
					// }

					// if (_clients[i].socket != NO_SOCKET && FD_ISSET(_clients[i].socket, &_except_fds)) 
					// {
					// 	printf("Exception client fd.\n");
					// 	close_client_connection(&_clients[i]);
					// 	continue;
					// }
				}



		}
	}
}

int Server::start_listen_socket()	//Prepares socket, CRITICAL
{
	int reuse = 1;
	//Creating socket to allow network connection
	if ((_listen_sock = socket(V4, TCP, IP)) == -1)
	{
		std::cerr << "SOCKET ERROR : START_LISTEN_SOCK\n";
		return (-1);
	}	
	if (setsockopt(_listen_sock, SOL_SOCKET, SO_REUSEADDR /*| SO_REUSEPORT*/, &reuse, sizeof(int)) != 0) 
	{
		std::cerr << "SOCKET ERROR : START_LISTEN_SOCK\n";
		return -1;
	}

	if (bind(_listen_sock,(struct sockaddr*)(&_addr), sizeof(struct sockaddr)) != 0) 
	{
		std::cerr << "SOCKET ERROR : START_LISTEN_SOCK\n";
		return -1;
	}
	// start accept client connections
	if (listen(_listen_sock, MAX_BACKLOG) != 0) 
	{
	  std::cerr << "SOCKET ERROR : START_LISTEN_SOCK\n";
		return -1;
	}
	if (DEBUG)
		std::cout << "Accepting connections on port : " << _port << std::endl;
 	return 0;
}

int Server::setup_fd_set()
{
	FD_ZERO(&_read_fds);
	FD_SET(_listen_sock, &_read_fds);
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (_clients[i].socket != -1)
			FD_SET(_clients[i].socket, &_read_fds);

	}
	FD_ZERO(&_write_fds);
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (_clients[i].socket != -1) //&& if ready ?? connection_list[i].send_buffer.current > 0))
			FD_SET(_clients[i].socket, &_write_fds);
	}

	FD_ZERO(&_except_fds);
	FD_SET(_listen_sock, &_except_fds);
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (_clients[i].socket != -1)
			FD_SET(_clients[i].socket, &_except_fds);
	}

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