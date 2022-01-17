/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 11:55:53 by hthomas           #+#    #+#             */
/*   Updated: 2022/01/17 12:24:16 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <fcntl.h>
Webserv::Webserv(const string config_file)
{
	parse_config(config_file);
	build();
}

Webserv::~Webserv()
{
	this->stop();
}

void	Webserv::push_back_server(Server *server)
{
	// DEBUG("PUSHING");
	_servers.push_back(server);
}

bool Webserv::conflict_ip_address_port_server_names(const string &new_ip_address, const unsigned int new_port, const list<string> &new_server_names) const
{
	for (list<Server*>::const_iterator server = _servers.begin(); server != _servers.end(); server++)
	{
		if ((*server)->get_ip_address() == new_ip_address && (*server)->get_port() == new_port)
		{
			list<string> x = (*server)->get_server_names();
			for (list<string>::iterator server_name = x.begin(); server_name != x.end(); server_name++)
			{
				for (list<string>::const_iterator new_server_name = new_server_names.begin(); new_server_name != new_server_names.end(); new_server_name++)
				{
					if (*server_name == *new_server_name)
						return true;
				}
			}
		}
	}
	return false;
}

void	Webserv::parse_config(const string config_file)
{
	if (!config_file.length())
	{
		Log("Please provide a config file");
		exit (1);
	}
	const vector<string> config = ft_split(get_content_file(config_file), "\n");
	if (!config.size())
	{
		Log("Config file is empty");
		exit (1);
	}
	// Parse and add multiple _servers in "_servers"
	size_t line_count = 0;
	while (line_count < config.size())
	{
		vector<string> line = ft_split(config[line_count], WHITESPACES);
		if (line[0] == "server")
		{
			Server *server = parse_server(config, &line_count);
			if (server && conflict_ip_address_port_server_names(server->get_ip_address(), server->get_port(), server->get_server_names()))
				err_parsing_config(server, "ip_address:port/server_names conflict with another server");
			// DEBUG("PUSHING SERV");
			push_back_server(server);
		}
		// DEBUG("\n**********\nline_count       : " << line_count);
		// DEBUG("config[line_count]:|" << config[line_count] << "|");
		line_count++;
	}
	Log("Config file loaded", GREEN);
	DEBUG("!!!!!!! CONFIG PARSED !!!!!!" << endl);
}

void Webserv::build(void)
{
	FD_ZERO(&listen_set);
	FD_ZERO(&write_set);

	high_fd = 0;
	int fd;
	//Setup the set for listening on different ports/IP
	for (list<Server*>::iterator server = _servers.begin(); server != _servers.end(); server++)
	{
		DEBUG("Runing on " << (*server)->get_ip_address() << ":" << (*server)->get_port());
		fd = (*server)->setup();
		FD_SET(fd, &listen_set);
		if (fd > high_fd)
			high_fd = fd;
	}
}


void Webserv::accept_new_conn(void)
{
	for (list<Server*>::iterator server = _servers.begin(); server != _servers.end(); server++)
	{
		if (FD_ISSET((*server)->get_listen_fd(), &lcopy_set))
		{
			Client *client = new Client((*server)->get_listen_fd());
			// DEBUG("PID OF NEW IS " << client->get_fd());
			// Log("New client connected", GREEN);

			client->push_back_server(*server);
			_clients.push_back(client);
			//TO MOVE


			FD_SET(client->get_fd(), &listen_set);
			FD_SET(client->get_fd(), &write_set);
		}
	}
}

void 	Webserv::loop_prep(void) //This can be optimized
{
	lcopy_set = listen_set;
	wcopy_set = write_set;

	int tmpfd;
	for (list<Client*>::iterator client = _clients.begin(); client != _clients.end(); client++)
	{
		tmpfd = (*client)->get_fd();

		if (tmpfd > high_fd)
			high_fd = tmpfd;
		else if (tmpfd == -1)
		{
			delete (*client);
			*client = 0;
			client = _clients.erase(client);
			if (client == _clients.end())
				break;
		}
	}
}

void Webserv::clear_fd(Client *client)
{
	FD_CLR(client->get_fd(), &listen_set);
	FD_CLR(client->get_fd(), &write_set);

	close(client->get_fd());
	client->set_fd(-1);
}

void	Webserv::listen(void)
{
	Log("Server started", GREEN);
	while (true)
	{
		// DEBUG("Waiting for new connections...");
		loop_prep();
		select(high_fd + 1, &lcopy_set, &wcopy_set, NULL, 0);
		accept_new_conn();

		for (list<Client*>::iterator client = _clients.begin(); client != _clients.end(); client++)
		{
			if (FD_ISSET((*client)->get_fd(), &lcopy_set))
			{
				if (((*client)->status() == 0 || (*client)->status() == 4) && (*client)->receive() == -1) //Or if we need more data to feed CGI
				{
						clear_fd(*client);
						delete (*client);
						client = _clients.erase(client);
						--client;
				}
			}
			else if ((*client)->is_done_recv())
			{

				if ((*client)->status() == 0 ||(*client)->status() == 4 )
				{
					DEBUG("SET RESP")
					(*client)->set_response();
				}
				else if ((*client)->status() > 0)
					(*client)->smart_send();
			}
			else if (fcntl((*client)->get_fd(), F_GETFL) < 0)
					DEBUG("AH, FOUND ONE");
		}
	}
}


void Webserv::stop(void)
{
	DEBUG("CLOSING");
	for (list<Server *>::iterator server = _servers.begin(); server != _servers.end(); server++)
	{
		DEBUG("DELETING SRVER");
		delete (*server);
		*server = 0;
	}
	_servers.clear();
	for (list<Client *>::iterator client = _clients.begin(); client != _clients.end(); client++)
	{
		delete (*client);
		*client = 0;
	}
	_clients.clear();
	DEBUG("CLOSED");
	exit(0);
}
