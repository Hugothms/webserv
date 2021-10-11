/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 11:55:53 by hthomas           #+#    #+#             */
/*   Updated: 2021/10/11 16:31:43 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool	parse_location(std::string config, size_t *pos, Location *returned_location)
{
	// Locations's attributes:
	std::string				location;
	std::list<std::string>	HTTP_methods;
	std::string				HTTP_redirection;
	std::string				location_root;
	bool 					directory_listing = false;
	std::string				default_answer;

	location = get_str_before_char(config, " ", pos);
	DEBUG("\t"<<location<<"\n\t{");
	if (get_str_before_char(config, " ;\n", pos) != "{")
		return false;
	std::string str;
	while ((str = get_str_before_char(config, " ;\n", pos)) != "}")
	{
		std::string tmp;

		DEBUG("\t\t"<<str<<":");
		if (str[0] == '#')
			get_str_before_char(config, "\n", pos);
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
			if ((HTTP_redirection = get_str_before_char(config, ";", pos)).length())
				get_str_before_char(config, "\n", pos);
			DEBUG("\t\t\t" << HTTP_redirection);
		}
		else if (str == "location_root")
		{
			if ((location_root = get_str_before_char(config, ";", pos)).length())
				get_str_before_char(config, "\n", pos);
			DEBUG("\t\t\t" << location_root);
		}
		else if (str == "directory_listing")
		{
			if ((tmp = get_str_before_char(config, ";", pos)).length())
			{
				directory_listing = atoi(tmp.c_str());
				get_str_before_char(config, "\n", pos);
			}
			DEBUG("\t\t\t" << directory_listing);
		}
		else if (str == "default_answer")
		{
			if ((default_answer = get_str_before_char(config, ";", pos)).length())
				get_str_before_char(config, "\n", pos);
			DEBUG("\t\t\t" << default_answer);
		}
		else
		{
			DEBUG("\t\t***OTHER_LOCATION: " << str);
			get_str_before_char(config, "\n", pos);
		}
		// sleep(1);
	}
	DEBUG("\t}");
	*returned_location = Location(location, HTTP_methods, HTTP_redirection, location_root, directory_listing, default_answer);
	return true;
}

Webserv::Webserv(std::string config_file)
{
	if (config_file == "")
	{
		DEBUG("Default config (no config provided)");
		servers.push_back(Server(std::list<Location>(), std::list<std::string>(), std::list<std::string>()));
		return ;
	}
	std::string config = get_content_file(config_file);
	DEBUG("Provided config:");
	DEBUG(config);

	// Parse and add multiple servers in "servers"
	size_t pos = 0;
	std::string str;
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
			std::list<Location>		locations;
			std::list<std::string>	server_names;
			std::list<std::string>	error_pages;
			unsigned int			port = 8080;
			std::string				root = "website";
			std::string				index = "index.html";
			unsigned int			max_client_body_size = 2048;

			while (config[pos] && (str = get_str_before_char(config, " ;\n", &pos)) != "}")
			{
				std::string	tmp;
				Location	location;

				DEBUG("\t"<<str);
				if (str.empty() || str[0] == '#')
					get_str_before_char(config, "\n", &pos);
				else if (str == "listen")
				{
					if((tmp = get_str_before_char(config, ";", &pos)).length())
					{
						port = atoi(tmp.c_str());
						get_str_before_char(config, "\n", &pos);
					}
					DEBUG("\t\t" << port);
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
					if((tmp = get_str_before_char(config, ";", &pos)).length())
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
					str = get_str_before_char(config, ";\n", &pos);
				}
			}
			DEBUG("}");
			servers.push_back(Server(locations, server_names, error_pages, port, root, index, max_client_body_size));
		}
	}
	DEBUG("******** PARSED ********\n");
}

void	Webserv::listen()
{
	while (true)
	{
		for (std::list<Server>::iterator server = servers.begin(); server != servers.end(); server++)
		{
			// DEBUG("\t---" << &server);
			server->s_listen();
		}
	}
}
