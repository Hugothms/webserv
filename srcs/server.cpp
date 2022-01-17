/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2022/01/17 16:51:42 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server()
: max_client_body_size(1024), listen_fd(0)
{}

Server::~Server()
{
	DEBUG("KILLING SERVER");
	close(listen_fd);
	DEBUG("KILLED");
}

string to_string_customa(const int &error_code)
{
	stringstream ret;
	ret << error_code;
	return ret.str();
}
int Server::setup(void)
{
	listen_fd = socket(AF_INET, SOCK_STREAM , 0);
	if (listen_fd == -1)
	{
		//Can't use ERRNO value
		perror("socket");
		exit(1);
	}
	Log("Listening socket created on port: " + to_string_customa(port), GREEN);

	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);


	//TODEL
	// if (ip_address.empty())
	// 	inet_pton(AF_INET, "0.0.0.0", &(hint.sin_addr));
	// else //TODEL
	inet_pton(AF_INET, ip_address.c_str(), &(hint.sin_addr));

	int opt = 1;
	//Need to check conn alive and stuff
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(listen_fd, (const struct sockaddr *)&hint, sizeof(hint)) == -1)
	{
		perror("bind");
		exit(1);
	}
	//Chanege max number of clients
	listen(listen_fd, SOMAXCONN);
	return (listen_fd);
}

bool	Server::is_valid(string &error) const
{
	if (get_ip_address() == "")
		error = "ip_address is not set";
	if (get_port() == 0)
		error = "port is not set";
	if (get_root() == "")
		error = "root is not set";
	if (get_index() == "")
		error = "index is not set";
	if (get_max_client_body_size() == 0)
		error = "max_client_body_size is not set";
	list<Location> locations = get_locations();
	if (!locations.size())
		error = "location is not set";
	for (list<Location>::iterator location = locations.begin(); location != locations.end(); location++)
	{
		if (!location->is_valid(error))
			return false;
	}
	return (error.length() == 0);
}

list<Location>	Server::get_locations() const
{
	return locations;
}

list<string>	Server::get_server_names() const
{
	return server_names;
}

map<string, string>	Server::get_cgis() const
{
	return cgis;
}

map<unsigned int, string>	Server::get_error_pages() const
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

void	Server::set_cgis(const map<string, string> cgis)
{
	this->cgis = cgis;
}

void	Server::push_back_cgi(const string extention_name, const string exec_path)
{
	this->cgis.insert(pair<string, string>(extention_name, exec_path));
}

void	Server::set_error_pages(const map<unsigned int, string> error_pages)
{
	this->error_pages = error_pages;
}

void	Server::push_back_error_page(const pair<unsigned int, string> error_page)
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
