/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/13 17:07:20 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server()
: host("localhost"), port(80), root("website"), index("index.html"), max_client_body_size(2048), _setup(false)
{}

Server::Server(	list<Location>	locations,
		list<string>			server_names,
		list<string>			error_pages,
		string					host,
		unsigned int			port,
		string					root,
		string					index,
		unsigned int			max_client_body_size)
		:locations(locations), server_names(server_names), error_pages(error_pages), host(host), port(port), root(root), index(index), max_client_body_size(max_client_body_size)
{}

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
			else if (i->fd == -1)
			{
				i = _clients.erase(i);
				if (i == _clients.end())
					break;
			}
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
				char buff[BUFFER_SIZE];
				int received_count = recv(i->fd, buff, BUFFER_SIZE, 0);
				if (received_count <= 0)
				{
					DEBUG("Client is done : ");
					i->identify();
					DEBUG("\n");
					FD_CLR(i->fd, &master_set);
					close(i->fd);
					i->fd = -1;
				}
				else
				{
					Request req(buff,received_count, i->fd);
					// DEBUG(buff);
					req.respond();
				}

			}
		}
	}
	// close(listen_fd);
}

unsigned int	Server::get_port()
{
	return port;
}

void	Server::set_locations(list<Location> locations)
{
	this->locations = locations;
}

void	Server::set_server_names(list<string> server_names)
{
	this->server_names = server_names;
}

void	Server::set_error_pages(list<string> error_pages)
{
	this->error_pages = error_pages;
}

void	Server::set_host(string host)
{
	this->host = host;
}

void	Server::set_port(unsigned int port)
{
	this->port = port;
}

void	Server::set_root(string root)
{
	this->root = root;
}

void	Server::set_index(string index)
{
	this->index = index;
}

void	Server::set_max_client_body_size(unsigned int max_client_body_size)
{
	this->max_client_body_size = max_client_body_size;
}
