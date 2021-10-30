/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/30 20:01:52 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server()
// : ip_address("0.0.0.0"), port(80), root("website"), index("index.html"), max_client_body_size(2048)
{}

Server::~Server()
{
	DEBUG("KILLING SERVER");
	close(listen_fd);
	DEBUG("KILLED");
}

// Client* Server::handle_new_conn()
// {
// 	DEBUG("New conn incomming, need to accept it !");

// 	Client *new_client = new Client();

// 	new_client->set_fd(accept(listen_fd, new_client->get_sockaddr(), new_client->get_addr_len()));

// 	inet_ntop(AF_INET, &(new_client->client_addr.sin_addr), new_client->client_ipv4_str, INET_ADDRSTRLEN);

// 	printf("Incoming connection from %s:%d.\n", new_client->v4str(), new_client->client_addr.sin_port);
// 	DEBUG("Client created !");
// 	return (new_client);
// }

int Server::setup(void)
{
	listen_fd = socket(AF_INET, SOCK_STREAM , 0);
	if (listen_fd == -1)
	{
		perror("socket");
		exit(1);
	}

	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);

	DEBUG("TRYING TO BIND TO " << ip_address);

	inet_pton(AF_INET, ip_address.c_str(), &(hint.sin_addr));

	int opt = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(listen_fd, (const struct sockaddr *)&hint, sizeof(hint)) == -1)
	{
		perror("bind");
		exit(1);
	}
	listen(listen_fd, SOMAXCONN);
	return (listen_fd);
}

list<Location>	Server::get_locations() const
{
	return locations;
}

list<string>	Server::get_server_names() const
{
	return server_names;
}

map<int, string>	Server::get_error_pages() const
{
	return error_pages;
}

string			Server::get_ip_address() const
{
	return ip_address;
}

unsigned int	Server::get_port() const
{
	return port;
}

string			Server::get_root() const
{
	return root;
}

string			Server::get_index() const
{
	return index;
}

unsigned int	Server::get_max_client_body_size() const
{
	return max_client_body_size;
}


int Server::get_listen_fd(void) const
{
	return listen_fd;
}
void	Server::set_locations(const list<Location> locations)
{
	this->locations = locations;
}

void	Server::push_back_location(Location location)
{
	this->locations.push_back(location);
}

void	Server::set_server_names(const list<string> server_names)
{
	this->server_names = server_names;
}

void	Server::push_back_server_name(string server_name)
{
	this->server_names.push_back(server_name);
}

void	Server::set_error_pages(const map<int, string> error_pages)
{
	this->error_pages = error_pages;
}

void	Server::push_back_error_page(pair<int, string> error_page)
{
	this->error_pages.insert(error_page);
}

void	Server::set_ip_address(const string ip_address)
{
	this->ip_address = ip_address;
}

void	Server::set_port(const unsigned int port)
{
	this->port = port;
}

void	Server::set_root(const string root)
{
	this->root = root;
}

void	Server::set_index(const string index)
{
	this->index = index;
}

void	Server::set_max_client_body_size(const unsigned int max_client_body_size)
{
	this->max_client_body_size = max_client_body_size;
}
