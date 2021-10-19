/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/19 16:40:16 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server()
: host("localhost"), port(PORT), root("website"), index("index.html"), max_client_body_size(2048)
{}

Server::~Server()
{
	close(listen_fd);
}

Client Server::handle_new_conn()
{
	DEBUG("New conn incomming, need to accept it !");

	Client new_client;

	new_client.fd = accept(listen_fd, new_client.get_sockaddr(), new_client.get_addr_len());
	// new_client.set_server(this);

	inet_ntop(AF_INET, &(new_client.client_addr.sin_addr), new_client.client_ipv4_str, INET_ADDRSTRLEN);
	printf("Incoming connection from %s:%d.\n", new_client.v4str(), new_client.client_addr.sin_port);
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

	// _setup = true;
	// high_fd = listen_fd;
	return listen_fd;
}

list<Location>	Server::get_locations()
{
	return locations;
}

list<string>	Server::get_server_names()
{
	return server_names;
}

map<int, string>	Server::get_error_pages()
{
	return error_pages;
}

string			Server::get_host()
{
	return host;
}

unsigned int	Server::get_port()
{
	return port;
}

string			Server::get_root()
{
	return root;
}

string			Server::get_index()
{
	return index;
}

unsigned int	Server::get_max_client_body_size()
{
	return max_client_body_size;
}


int Server::get_listen_fd(void)
{
	return listen_fd;
}
void	Server::set_locations(list<Location> locations)
{
	this->locations = locations;
}

void	Server::push_back_location(Location location)
{
	this->locations.push_back(location);
}

void	Server::set_server_names(list<string> server_names)
{
	this->server_names = server_names;
}

void	Server::push_back_server_name(string server_name)
{
	this->server_names.push_back(server_name);
}

void	Server::set_error_pages(map<int, string> error_pages)
{
	this->error_pages = error_pages;
}

void	Server::push_back_error_page(pair<int, string> error_page)
{
	this->error_pages.insert(error_page);
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
