/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 11:55:53 by hthomas           #+#    #+#             */
/*   Updated: 2021/10/20 08:20:14 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool Webserv::is_a_valid_server(list<Location>	locations,
								list<string>	server_names,
								list<string>	error_pages,
								string			host,
								unsigned int 	port,
								string			root,
								string			index,
								unsigned int	max_client_body_size)
{
	// TODO
	(void) host;
	(void) port;
	(void) root;
	(void) index;
	(void) max_client_body_size;
	if (!locations.size() || !server_names.size() || !error_pages.size())
		return false;
	return true;
}

bool Webserv::conflict_host_port_server_names(string new_host, unsigned int new_port, list<string> new_server_names)
{
	for (list<Server*>::iterator server = _servers.begin(); server != _servers.end(); server++)
	{
		if ((*server)->get_host() == new_host && (*server)->get_port() == new_port)
		{
			list<string> x = (*server)->get_server_names();
			for (list<string>::iterator server_name = x.begin(); server_name != x.end(); server_name++)
			{
				for (list<string>::iterator new_server_name = new_server_names.begin(); new_server_name != new_server_names.end(); new_server_name++)
				{
					if (*server_name == *new_server_name)
						return true;
				}
			}
		}
	}
	return false;
}

void	err_parsing_config(string error)
{
	cerr << "Error: Wrong server configuration: " << error << endl;
	exit(5);
}

Location	parse_location(string config, size_t *pos)
{
	Location	location;
	string		tmp;

	tmp = get_str_before_char(config, " ", pos);
	DEBUG("\t" << tmp << "\n\t{");
	if (get_str_before_char(config, " ;\n", pos) != "{")
		err_parsing_config("expecting '{' after 'server'");
	while ((tmp = get_str_before_char(config, " ;\n", pos)) != "}")
	{
		if (tmp[0] == '#')
		{
			if (config[*pos-1] != '\n')
				get_str_before_char(config, "\n", pos);
		}
		else if (tmp.length())
			DEBUG("\t\t" << tmp << ":");
		if (tmp == "HTTP_methods")
		{
			while ((tmp = get_str_before_char(config, " ;", pos)).length())
			{
				DEBUG("\t\t\t" << tmp);
				location.push_back_HTTP_method(tmp);
			}
			get_str_before_char(config, "\n", pos);
		}
		else if (tmp == "HTTP_redirection")
		{
			if ((tmp = get_str_before_char(config, ";", pos)).length())
			{
				location.set_HTTP_redirection(tmp);
				DEBUG("\t\t\t" << tmp);
				get_str_before_char(config, "\n", pos);
			}
		}
		else if (tmp == "location_root")
		{
			if ((tmp = get_str_before_char(config, ";", pos)).length())
			{
				location.set_location_root(tmp);
				DEBUG("\t\t\t" << tmp);
				get_str_before_char(config, "\n", pos);
			}
		}
		else if (tmp == "default_answer")
		{
			if ((tmp = get_str_before_char(config, ";", pos)).length())
			{
				location.set_default_answer(tmp);
				DEBUG("\t\t\t" << tmp);
				get_str_before_char(config, "\n", pos);
			}
		}
		else if (tmp == "directory_listing")
		{
			if ((tmp = get_str_before_char(config, ";", pos)) == "0" || tmp == "1")
			{
				location.set_directory_listing(atoi(tmp.c_str()));
				DEBUG("\t\t\t" << tmp);
				get_str_before_char(config, "\n", pos);
			}
		}
		else if (tmp.length())
		{
			// DEBUG("\t\t***OTHER_LOCATION: " << tmp);
			get_str_before_char(config, ";\n", pos);
		}
		// sleep(1);
	}
	DEBUG("\t}");
	return location;
}

Webserv::Webserv(string config_file)
{
	if (config_file == "")
	{
		DEBUG("Default config (no config provided)");
		_servers.push_back(new Server());
		return ;
	}
	const string config = get_content_file(config_file);
	DEBUG("Provided config:\n" << config);

	// Parse and add multiple _servers in "_servers"
	size_t pos = 0;
	while (config[pos]) // config parsing loop
	{
		string tmp = get_str_before_char(config, " \n", &pos);
		if (tmp == "server")
		{
			DEBUG("!!!!!!!!!! SERVER !!!!!!!!!!");
			Server *server = new Server();
			tmp = get_str_before_char(config, "\n", &pos);
			if (tmp != "{")
				continue ;
			DEBUG("{");
			while (config[pos] && (tmp = get_str_before_char(config, " ;\n", &pos)) != "}")
			{
				DEBUG("\t" << tmp);
				if (tmp.empty() || (tmp[0] == '#'))
				{
					if (tmp[1])
						get_str_before_char(config, "\n", &pos);
				}
				else if (tmp == "location")
					server->push_back_location(parse_location(config, &pos));
				else if (tmp == "server_name")
				{
					while ((tmp = get_str_before_char(config, " ;", &pos)).length())
					{
						DEBUG("\t\t" << tmp);
						server->push_back_server_name(tmp);
					}
					get_str_before_char(config, "\n", &pos);
				}
				else if (tmp == "error_page")
				{
					if ((tmp = get_str_before_char(config, " =;", &pos)).length())
					{
						int error = atoi(tmp.c_str());
						if (config[pos] != '=')
							err_parsing_config("error page not well configured");
						else
							pos++;
						if ((tmp = get_str_before_char(config, ";", &pos)).length())
						{
							server->push_back_error_page(pair<int, string>(error, tmp));
							DEBUG("\t\t" << error << " = " << tmp);
						}
					}
					get_str_before_char(config, "\n", &pos);
				}
				else if (tmp == "listen")
				{
					if (!(tmp = get_str_before_char(config, ":", &pos)).length())
						tmp = "0.0.0.0";
					server->set_host(tmp);
					DEBUG("\t\thost: " << tmp);
					if (is_integer(tmp = get_str_before_char(config, ";", &pos)))
					{
						server->set_port(atoi(tmp.c_str()));
						DEBUG("\t\tport: " << atoi(tmp.c_str()));
					}
					get_str_before_char(config, "\n", &pos);
				}
				else if (tmp == "root")
				{
					if((tmp = get_str_before_char(config, ";", &pos)).length())
					{
						server->set_root(tmp);
						get_str_before_char(config, "\n", &pos);
					}
					DEBUG("\t\t" << server->get_root());
				}
				else if (tmp == "index")
				{
					if((tmp = get_str_before_char(config, ";", &pos)).length())
					{
						server->set_index(tmp);
						get_str_before_char(config, "\n", &pos);
					}
					DEBUG("\t\t" << server->get_index());
				}
				else if (tmp == "max_client_body_size")
				{
					if(is_integer(tmp = get_str_before_char(config, ";", &pos)))
					{
						get_str_before_char(config, "\n", &pos);
						server->set_max_client_body_size(atoi(tmp.c_str()));
					}
					DEBUG("\t\t" << server->get_max_client_body_size());
				}
				else if (tmp.length())
				{
					DEBUG("\t\t***OTHER: " << tmp);
					tmp = get_str_before_char(config, "\n", &pos);
				}
			}
			DEBUG("}");
			if (conflict_host_port_server_names(server->get_host(), server->get_port(), server->get_server_names()))
				err_parsing_config("host:port/server_names conflict with another server");

			_servers.push_back(server);
			DEBUG(_servers.back()->get_root() << "\n");
			DEBUG(_servers.back()->get_host() << "\n");
		}
	}
	DEBUG("******* CONFIG PARSED ******\n");
}

void Webserv::build()
{
	FD_ZERO(&master_set);
	high_fd = 0;
	int fd;
	//Setup the set for listening on different ports/IP
	for (list<Server*>::iterator server = _servers.begin(); server != _servers.end(); server++)
	{
		DEBUG("Run for " << (*server)->get_host() << ":" << (*server)->get_port());
		fd = (*server)->setup();
		FD_SET(fd, &master_set);
		if (fd > high_fd)
			high_fd = fd;
		DEBUG("Port added to the FD_SET !\n");
	}
}

void Webserv::process(Client *client)
{
	char buff[BUFFER_SIZE];
	int len = recv(client->fd, buff, BUFFER_SIZE, 0);

	Server *target = 0;

	if (len > 0)
	{
		Request req(buff,len, client->fd);
		req.respond(client->servers);
		// req.respond(*client->server);
	}
	else
	{
		DEBUG("Client is done\n");
		FD_CLR(client->fd, &master_set);
		close(client->fd);
		client->fd = -1;
	}
}

void 	Webserv::sig()
{
	return ;
}

void	Webserv::listen()
{

	build();
	copy_set = master_set;
	while (true)
	{
		// DEBUG("START");
		//This can be optimized
		for (list<Client*>::iterator client = _clients.begin(); client != _clients.end(); client++)
		{
			int tmpfd = (*client)->get_fd();
			if (tmpfd > high_fd)
				high_fd = tmpfd;
			else if (tmpfd == -1)
			{
				client = _clients.erase(client);
				if (client == _clients.end())
					break;
			}
		}

		copy_set = master_set;

		select(high_fd + 1, &copy_set, NULL, NULL, 0);

		// Accept new clients on each server
		for (list<Server*>::iterator server = _servers.begin(); server != _servers.end(); server++)
		{
			if (FD_ISSET((*server)->get_listen_fd(), &copy_set))
			{

				Client *client = (*server)->handle_new_conn();
				client->push_back_server(*server);

				_clients.push_back(client);
				FD_SET(client->get_fd(), &master_set);
			}
		}
		// Loop through all the clients and find out if they sent
		for (list<Client*>::iterator client = _clients.begin(); client != _clients.end(); client++)
		{
			if (FD_ISSET((*client)->get_fd(), &copy_set))
				process(*client);
			if ((*client)->get_fd() == -1)
			{
				delete (*client);
				client = _clients.erase(client);
				--client;
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
	// _servers.clear();
	// _clients.clear();
	DEBUG("CLOSED");
}

Webserv::~Webserv()
{
	// for (list<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	// {
	// 	close(it->fd);
	// }
	// _servers.erase(_servers.begin(), _servers.end());
}
