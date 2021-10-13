/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 11:55:53 by hthomas           #+#    #+#             */
/*   Updated: 2021/10/13 21:00:26 by edal--ce         ###   ########.fr       */
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
	if (!locations.size() || !server_names.size() || !error_pages.size())
		return false;
	return true;
}

void	err_parsing_config()
{
	cerr << "Wrong server configuration" << endl;
	exit(5);
}

bool	parse_location(string config, size_t *pos, Location *returned_location)
{
	// Locations's attributes:
	string			location;
	list<string>	HTTP_methods;
	string			HTTP_redirection;
	string			location_root;
	bool 			directory_listing = false;
	string			default_answer;

	location = get_str_before_char(config, " ", pos);
	DEBUG("\t" << location << "\n\t{");
	if (get_str_before_char(config, " ;\n", pos) != "{")
		return false;
	string str;
	while ((str = get_str_before_char(config, " ;\n", pos)) != "}")
	{
		string tmp;

		if (str[0] != '#')
			DEBUG("\t\t" << str << ":");
		if (str[0] == '#')
		{
			if (config[*pos-1] != '\n')
				get_str_before_char(config, "\n", pos);
		}
		else if (str == "HTTP_methods")
		{
			while ((tmp = get_str_before_char(config, " ;", pos)).length())
			{
				DEBUG("\t\t\t" << tmp);
				HTTP_methods.push_back(tmp);
			}
			get_str_before_char(config, "\n", pos);
		}
		else if (str == "HTTP_redirection")
		{
			DEBUG("\t\t\t" << HTTP_redirection);
			if ((HTTP_redirection = get_str_before_char(config, ";", pos)).length())
				get_str_before_char(config, "\n", pos);
		}
		else if (str == "location_root")
		{
			DEBUG("\t\t\t" << location_root);
			if ((location_root = get_str_before_char(config, ";", pos)).length())
				get_str_before_char(config, "\n", pos);
		}
		else if (str == "directory_listing")
		{
			if ((tmp = get_str_before_char(config, ";", pos)) == "0" || tmp == "1")
			{
				directory_listing = atoi(tmp.c_str());
				DEBUG("\t\t\t" << directory_listing);
				get_str_before_char(config, "\n", pos);
			}
		}
		else if (str == "default_answer")
		{
			DEBUG("\t\t\t" << default_answer);
			if ((default_answer = get_str_before_char(config, ";", pos)).length())
				get_str_before_char(config, "\n", pos);
		}
		else
		{
			// DEBUG("\t\t***OTHER_LOCATION: " << str);
			get_str_before_char(config, ";\n", pos);
		}
		// sleep(1);
	}
	DEBUG("\t}");
	*returned_location = Location(location, HTTP_methods, HTTP_redirection, location_root, directory_listing, default_answer);
	return true;
}

Webserv::Webserv(string config_file)
{
	if (config_file == "")
	{
		DEBUG("Default config (no config provided)");
		_servers.push_back(Server());
		return ;
	}
	const string config = get_content_file(config_file);
	DEBUG("Provided config:\n" << config);

	// Parse and add multiple _servers in "_servers"
	size_t pos = 0;
	string str;
	while (config[pos]) // config parsing loop
	{
		str = get_str_before_char(config, " \n", &pos);
		if (str == "server")
		{
			DEBUG("!!!!!!!!!! SERVER !!!!!!!!!!");
			Server server;
			str = get_str_before_char(config, "\n", &pos);
			if (str != "{")
				continue ;
			DEBUG("{");
			while (config[pos] && (str = get_str_before_char(config, " ;\n", &pos)) != "}")
			{
				string		tmp;
				Location	location;

				DEBUG("\t" << str);
				if (str.empty() || (str[0] == '#'))
				{
					if (str[1])
						get_str_before_char(config, "\n", &pos);
				}
				else if (str == "location" && parse_location(config, &pos, &location))
					server.push_back_location(location);
				else if (str == "server_name")
				{
					while ((tmp = get_str_before_char(config, " ;", &pos)).length())
					{
						DEBUG("\t\t" << tmp);
						server.push_back_server_name(tmp);
					}
					get_str_before_char(config, "\n", &pos);
				}
				else if (str == "error_pages")
				{
					while ((tmp = get_str_before_char(config, " ;", &pos)).length())
					{
						server.push_back_error_page(tmp);
						get_str_before_char(config, "\n", &pos);
					}
				}
				else if (str == "listen")
				{
					server.set_host(get_str_before_char(config, ":", &pos));
					DEBUG("\t\thost " << server.get_host());
					if(is_integer(tmp = get_str_before_char(config, ";", &pos)))
						server.set_port(atoi(tmp.c_str()));
					get_str_before_char(config, "\n", &pos);
					DEBUG("\t\tport: " << server.get_port());
				}
				else if (str == "root")
				{
					if((tmp = get_str_before_char(config, ";", &pos)).length())
					{
						server.set_root(tmp);
						get_str_before_char(config, "\n", &pos);
					}
					DEBUG("\t\t" << server.get_root());
				}
				else if (str == "index")
				{
					if((tmp = get_str_before_char(config, ";", &pos)).length())
					{
						server.set_index(tmp);
						get_str_before_char(config, "\n", &pos);
					}
					DEBUG("\t\t" << server.get_index());
				}
				else if (str == "max_client_body_size")
				{
					if(is_integer(tmp = get_str_before_char(config, ";", &pos)))
					{
						get_str_before_char(config, "\n", &pos);
						server.set_max_client_body_size(atoi(tmp.c_str()));
					}
					DEBUG("\t\t" << server.get_max_client_body_size());
				}
				else
				{
					DEBUG("\t\t***OTHER: " << str);
					str = get_str_before_char(config, "\n", &pos);
				}
			}
			DEBUG("}");
			if (!is_a_valid_server(server.get_locations(), server.get_server_names(), server.get_error_pages(), server.get_host(), server.get_port(), server.get_root(), server.get_index(), server.get_max_client_body_size()))
			{
				cerr << "Wrong server configuration" << endl;
				exit(5);
			}
			_servers.push_back(server);
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
	for (list<Server>::iterator server = _servers.begin(); server != _servers.end(); server++)
	{
		DEBUG("Run for port: " << server->get_port());
		DEBUG(server->get_host());
		DEBUG(server->get_host());
		fd = server->setup();
		FD_SET(fd, &master_set);
		if (fd > high_fd)
			high_fd = fd;
		DEBUG("Port added to the FD_SET !\n");
	}
}

void Webserv::process(Client *i)
{
	char buff[BUFFER_SIZE];
	int len = recv(i->fd, buff, BUFFER_SIZE, 0);
	if (len > 0)
	{
		Request req(buff,len, i->fd);
		req.respond();					
	}
	else
	{
		DEBUG("Client is done\n");
		FD_CLR(i->fd, &master_set);
		close(i->fd);
		i->fd = -1;

	}
}


void	Webserv::listen()
{
	build();
	copy_set = master_set;
	while (true)
	{	
		// DEBUG("START");
		//This can be optimized
		for (list<Client>::iterator i = _clients.begin(); i != _clients.end(); i++)
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

		copy_set = master_set;
		
		select(high_fd + 1, &copy_set, NULL, NULL, 0);
		
		//Accept new clients on each server
		for (list<Server>::iterator i = _servers.begin(); i != _servers.end(); i++)
		{
			if (FD_ISSET(i->get_listen_fd(), &copy_set))
			{
				Client tmp = i->handle_new_conn();
				// tmp.set_server(&(*i));
				_clients.push_back(tmp);
				FD_SET(tmp.fd, &master_set);
			}
		}
		// //Loop through all the clients and find out if they sent
		for (list<Client>::iterator i = _clients.begin(); i != _clients.end(); i++)
		{
			if (FD_ISSET(i->fd, &copy_set))
				process(&(*i));
		}
	}
}

Webserv::~Webserv()
{
	for (list<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		close(it->fd);
	}
	_servers.erase(_servers.begin(), _servers.end());
}
