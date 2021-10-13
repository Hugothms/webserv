/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 11:55:53 by hthomas           #+#    #+#             */
/*   Updated: 2021/10/13 17:33:52 by hthomas          ###   ########.fr       */
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
		servers.push_back(Server());
		return ;
	}
	const string config = get_content_file(config_file);
	DEBUG("Provided config:");
	DEBUG(config);

	// Parse and add multiple servers in "servers"
	size_t pos = 0;
	string str;
	while (config[pos]) // config parsing loop
	{
		DEBUG("!!!!!!!!!!!! SERVER !!!!!!!!!!!!");
		str = get_str_before_char(config, " ", &pos);
		if (str == "server")
		{
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
			servers.push_back(server);
		}
	}
	DEBUG("******* CONFIG PARSED ******\n");
}

void	Webserv::listen()
{
	// servers.begin()->setup();
	// servers.begin()->run();
	while (true)
	{
		for (list<Server>::iterator server = servers.begin(); server != servers.end(); server++)
		{
			DEBUG("Run for port: " << server->get_port() << "\n");
			// DEBUG("\t---" << &server);
			server->run();
		}
	}
}
