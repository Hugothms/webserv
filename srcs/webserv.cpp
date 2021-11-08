/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 11:55:53 by hthomas           #+#    #+#             */
/*   Updated: 2021/11/08 13:58:23 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Webserv::Webserv(const string config_file)
{
	parse_config(config_file);
}

Webserv::~Webserv()
{
	stop();
}

void	Webserv::push_back_server(Server *server)
{
	_servers.push_back(server);
}

bool Webserv::conflict_ip_address_port_server_names(const string new_ip_address, const unsigned int new_port, const list<string> new_server_names) const
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
	if (config_file == "")
	{
		DEBUG("Default config (no config provided)");
		Server *srv = new Server();
		srv->set_port(80);
		push_back_server(srv);
		return ;
	}
	const string config = get_content_file(config_file);
	DEBUG("Provided config:" << endl << config);

	// Parse and add multiple _servers in "_servers"
	size_t pos = 0;
	while (config[pos]) // config parsing loop
	{
		string tmp = get_str_before_char(config, " \n", &pos);
		if (tmp == "server")
		{
			Server *server = parse_server(config, &pos);
			if (server && conflict_ip_address_port_server_names(server->get_ip_address(), server->get_port(), server->get_server_names()))
				err_parsing_config("ip_address:port/server_names conflict with another server");
			push_back_server(server);
		}
	}
	DEBUG("!!!!!!! CONFIG PARSED !!!!!!" << endl);
}

void Webserv::build()
{
	FD_ZERO(&listen_set);
	FD_ZERO(&write_set);
	high_fd = 0;
	int fd;
	//Setup the set for listening on different ports/IP
	for (list<Server*>::iterator server = _servers.begin(); server != _servers.end(); server++)
	{
		DEBUG("Run for " << (*server)->get_ip_address() << ":" << (*server)->get_port());
		fd = (*server)->setup();
		FD_SET(fd, &listen_set);
		if (fd > high_fd)
			high_fd = fd;
		DEBUG("Port added to the FD_SET !");
	}
}


void Webserv::accept_new_conn(void)
{
	for (list<Server*>::iterator server = _servers.begin(); server != _servers.end(); server++)
	{
		if (FD_ISSET((*server)->get_listen_fd(), &lcopy_set))
		{
			Client *client = new Client(*server);
			client->push_back_server(*server);
			_clients.push_back(client);
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
			client = _clients.erase(client);
			if (client == _clients.end())
				break;
		}
	}
}

void	Webserv::listen()
{
	build();
	while (true)
	{
		// DEBUG("Waiting for new connections...");
		loop_prep();
		select(high_fd + 1, &lcopy_set, &wcopy_set, NULL, 0);
		accept_new_conn();
		int i = 0;
		// DEBUG("AMT CLIENT " << _clients.size());
		for (list<Client*>::iterator client = _clients.begin(); client != _clients.end(); client++)
		{
			if (FD_ISSET((*client)->get_fd(), &lcopy_set)) //Case where there is stuff to read
			{
				DEBUG("client seems ready to transmit data");			
				
				if ((*client)->receive() == -1)
				{
					DEBUG("client seems to have left, clearing his marks");
					
					close((*client)->get_fd());
					FD_CLR((*client)->get_fd(), &listen_set);
					FD_CLR((*client)->get_fd(), &write_set);
					(*client)->set_fd(-1);
				}
			}
			else if ((*client)->is_done_recv())
			{
				DEBUG("client is done receiving");
				if ((*client)->send_rdy == 0)
				{
					Request req((*client)->get_rec_buff()->c_str(),(*client)->get_rec_buff()->length(), (*client)->get_fd());
					(*client)->set_response(req.respond((*client)->servers));
				}
				else if ((*client)->is_done_send() == 0)
				{
					(*client)->send();
				}
			}
		}
	}
}

void Webserv::stop()
{
	DEBUG("CLOSING");
	for (list<Server *>::iterator server = _servers.begin(); server != _servers.end(); server++)
	{
		delete (*server);
		*server = 0;
	}
	for (list<Client *>::iterator client = _clients.begin(); client != _clients.end(); client++)
	{
		delete (*client);
		*client = 0;
	}
	DEBUG("CLOSED");
}
