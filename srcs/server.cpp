/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/06 22:47:15 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "server.hpp"

// #include <sys/socket.h>
using namespace std;
#include <fcntl.h>

Server::~Server() 
{
	// delete _clients;
}
Server::Server(int prt)
{
	port = prt;
}

// Client Server::handle_new_conn(int listen_sock)
// {
// 	cout << "New conn incomming, need to accept it !\n";
	
// 	Client new_client;
	
// 	new_client.fd = accept(listen_sock, new_client.get_sockaddr(), new_client.get_addr_len());
	
// 	inet_ntop(AF_INET, &(new_client.client_addr.sin_addr), new_client.client_ipv4_str, INET_ADDRSTRLEN);
// 	// printf("Incoming connection from %s:%d.\n", new_client.v4str(), new_client.client_addr.sin_port);
			
// 	// clients.push_back(new_client);
// 	return (new_client);
// }

Server::Client Server::handle_new_conn(int fd)
{
	cout << "New conn incomming, need to accept it !\n";
	
	Client new_client;
	
	new_client.fd = accept(listen_fd, new_client.get_sockaddr(), new_client.get_addr_len());
	
	inet_ntop(AF_INET, &(new_client.client_addr.sin_addr), new_client.client_ipv4_str, INET_ADDRSTRLEN);
	// printf("Incoming connection from %s:%d.\n", new_client.v4str(), new_client.client_addr.sin_port);
			
	// clients.push_back(new_client);
	return (new_client);
}


int Server::setup(void)
{
	//Create IPV4 TCP socket;
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd == -1)
	{
		cerr << "Socket error\n";
		return -1;
	}

	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, "0.0.0.0", &(hint.sin_addr));

	if (bind(listen_fd, (const struct sockaddr *)&hint, sizeof(hint)) == -1)
	{
		cerr << "Bind error\n";
		return -2;
	}
	listen(listen_fd, SOMAXCONN);
	
	FD_ZERO(&master_set);
	
	FD_SET(listen_fd, &master_set);

	int high_fd = listen_fd;
	while(true)
	{
		copy_set = master_set;

		//This can be optimized
		for (int i = 0; i < _clients.size(); i++)
		{
			if (_clients[i].fd > high_fd)
				high_fd = _clients[i].fd;	
		}

		int sock_count = select(high_fd + 1, &copy_set, NULL, NULL, NULL);
		if (FD_ISSET(listen_fd, &copy_set))
		{
			Client tmp = handle_new_conn(listen_fd);
			_clients.push_back(tmp);
			std::cout << "Client added to the list : " ;
			_clients.back().identify();
			//Add the client FD to master for processing
			FD_SET(tmp.fd, &master_set);
		}
		// //Loop through all the clients and find out if they sent
		for (int i = 0; i < _clients.size(); i++)
		{
			if (FD_ISSET(_clients[i].fd, &copy_set))
			{
				_clients[i].identify();
				char buff[BUFFER_SIZE];
				int received_count = recv(_clients[i].fd, buff, BUFFER_SIZE, 0);
				write(1, buff, received_count);
				// write(_clients[i].fd,buff , received_count);
				Request req(buff,received_count, _clients[i].fd);
				req.respond();
			}
		}
	}
	close(listen_fd);
	return 0;
}