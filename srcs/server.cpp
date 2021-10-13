/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/13 15:30:43 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

Server::Server(	list<Location>	locations,
		list<string>			server_names,
		list<string>			error_pages,
		unsigned int			port,
		string					root,
		string					index,
		unsigned int			max_client_body_size)
		:locations(locations), server_names(server_names), error_pages(error_pages), port(port), root(root), index(index), max_client_body_size(max_client_body_size)
{
	_setup = false;
}

Server::~Server()
{
	// delete _clients;
	close(listen_fd);
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		close(_clients[i].fd);
	}
	_clients.clear();
	close(listen_fd);
}

unsigned int Server::get_port()
{
	return port;
}

Client Server::handle_new_conn()
{
	DEBUG("New conn incomming, need to accept it !");

	Client new_client;

	new_client.fd = accept(listen_fd, new_client.get_sockaddr(), new_client.get_addr_len());

	inet_ntop(AF_INET, &(new_client.client_addr.sin_addr), new_client.client_ipv4_str, INET_ADDRSTRLEN);
	// printf("Incoming connection from %s:%d.\n", new_client.v4str(), new_client.client_addr.sin_port);
	DEBUG("Client created !");
	// clients.push_back(new_client);
	return (new_client);
}

int Server::setup(void)
{
	//Create IPV4 TCP socket;
	listen_fd = socket(AF_INET, SOCK_STREAM , 0);
	if (listen_fd == -1)
	{
		perror("socket");
		exit(1);
	}

	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, "0.0.0.0", &(hint.sin_addr));

	int opt = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));


	if (bind(listen_fd, (const struct sockaddr *)&hint, sizeof(hint)) == -1)
	{
		perror("bind");
		exit(1);
	}

	listen(listen_fd, SOMAXCONN);

	FD_ZERO(&master_set);

	FD_SET(listen_fd, &master_set);

	_setup = true;
	high_fd = listen_fd;
	return 0;
}

int Server::run(void)
{
	if (_setup == false)
		setup();
	while(true)
	{
		copy_set = master_set;

		//This can be optimized
		for (vector<Client>::iterator i = _clients.begin(); i != _clients.end(); i++)
		{
			if (i->fd > high_fd)
				high_fd = i->fd;
		}

		select(high_fd + 1, &copy_set, NULL, NULL, NULL);
		if (FD_ISSET(listen_fd, &copy_set))
		{
			Client tmp = handle_new_conn();
			_clients.push_back(tmp);
			DEBUG("Client added to the list: ");
			// _clients.back().identify();
			//Add the client FD to master for processing
			FD_SET(tmp.fd, &master_set);
			DEBUG("Client added to the FD_SET: ");
		}
		// //Loop through all the clients and find out if they sent
		for (vector<Client>::iterator i = _clients.begin(); i != _clients.end(); i++)
		{
			if (FD_ISSET(i->fd, &copy_set))
			{
				i->identify();
				char buff[BUFFER_SIZE];
				int received_count = recv(i->fd, buff, BUFFER_SIZE, 0);
				if (received_count == 0)
				{
					cerr << "Client is done\n";
				}
				else if(received_count == -1)
				{
					DEBUG("FAUT GERER CA");
					DEBUG("-----received_count: " << received_count);
					// exit(6);
					perror("recv");
				}
				// DEBUG(buff);
				// write(_clients[i].fd,buff , received_count);
				Request req(buff,received_count, i->fd);
				req.respond();
			}
		}
	}
	// close(listen_fd);
}
