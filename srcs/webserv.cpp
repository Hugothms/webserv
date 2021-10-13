/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 11:55:53 by hthomas           #+#    #+#             */
/*   Updated: 2021/10/13 12:24:56 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

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
		servers.push_back(Server(list<Location>(), list<string>(), list<string>()));
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
			str = get_str_before_char(config, "\n", &pos);
			if (str != "{")
				continue ;
			DEBUG("{");
			// Webserv's attributes:
			list<Location>	locations;
			list<string>	server_names;
			list<string>	error_pages;
			string			host;
			unsigned int	port = 8080;
			string			root = "website";
			string			index = "index.html";
			unsigned int	max_client_body_size = 2048;

			while (config[pos] && (str = get_str_before_char(config, " ;\n", &pos)) != "}")
			{
				string	tmp;
				Location	location;

				DEBUG("\t" << str);
				if (str.empty() || (str[0] == '#'))
				{
					if (str[1])
						get_str_before_char(config, "\n", &pos);
				}
				else if (str == "listen")
				{
					host = get_str_before_char(config, ":", &pos);
					DEBUG("\t\thost " << host);
					if(is_integer(tmp = get_str_before_char(config, ";", &pos)))
						port = atoi(tmp.c_str());
					get_str_before_char(config, "\n", &pos);
					DEBUG("\t\tport: " << port);
				}
				else if (str == "server_name")
				{
					while ((tmp = get_str_before_char(config, " ;", &pos)).length())
					{
						DEBUG("\t\t" << tmp);
						server_names.push_back(tmp);
					}
					get_str_before_char(config, "\n", &pos);
				}
				else if (str == "root")
				{
					if((root = get_str_before_char(config, ";", &pos)).length())
						get_str_before_char(config, "\n", &pos);
					DEBUG("\t\t" << root);
				}
				else if (str == "index")
				{
					if((index = get_str_before_char(config, ";", &pos)).length())
						get_str_before_char(config, "\n", &pos);
					DEBUG("\t\t" << index);
				}
				else if (str == "error_pages")
				{
					while ((tmp = get_str_before_char(config, " ;", &pos)).length())
					{
						error_pages.push_back(tmp);
						get_str_before_char(config, "\n", &pos);
					}
				}
				else if (str == "max_client_body_size")
				{
					if(is_integer(tmp = get_str_before_char(config, ";", &pos)))
					{
						get_str_before_char(config, "\n", &pos);
						max_client_body_size = atoi(tmp.c_str());
					}
					DEBUG("\t\t" << max_client_body_size);
				}
				else if (str == "location" && parse_location(config, &pos, &location))
					locations.push_back(location);
				else
				{
					DEBUG("\t\t***OTHER: " << str);
					str = get_str_before_char(config, "\n", &pos);
				}
			}
			DEBUG("}");
			if (!is_a_valid_server(locations, server_names, error_pages, port, root, index, max_client_body_size))
			{
				cerr << "Wrong server configuration" << endl;
				exit(5);
			}
			servers.push_back(Server(locations, server_names, error_pages, port, root, index, max_client_body_size));
		}
	}
	DEBUG("******** PARSED ********\n");
}

void	Webserv::listen()
{
	// servers.begin()->setup();
	// servers.begin()->run();
	while (true)
	{
		for (list<Server>::iterator server = servers.begin(); server != servers.end(); server++)
		{
			DEBUG("Run for port: " << server->get_port());
			// DEBUG("\t---" << &server);
			server->run();
		}
	}
}
