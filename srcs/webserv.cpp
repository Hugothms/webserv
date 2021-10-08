/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 11:55:53 by hthomas           #+#    #+#             */
/*   Updated: 2021/10/08 17:11:44 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Webserv::Webserv(std::string config_file)
{

	if (config_file == "")
	{
		DEBUG("Default config (no config provided)");
		servers.push_back(Server(std::list<std::string>(), std::list<std::string>()));
		return ;
	}
	std::string config = get_content_file(config_file);
	DEBUG("Provided config:\n----------------");
	DEBUG(config);

	// Parse and add multiple servers in "servers"
	size_t pos = 0;
	std::string str;
	while (config[pos]) // config parsing loop
	{
		DEBUG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		str = get_str_before_char(config, " ", &pos);
		if (str == "server")
		{
			str = get_str_before_char(config, "\n", &pos);
			DEBUG("SERVER");
			if (str != "{")
				continue ;
			// To be parsed:
			std::list<std::string>	server_names;
			std::list<std::string>	error_pages;
			unsigned int			port = 8080;
			std::string				root = "website";
			std::string				index = "index.html";
			unsigned int			max_client_body_size = 2048;

			while (config[pos] && str != "}")
			{
				str = get_str_before_char(config, " ", &pos);
				DEBUG(str);

				if (str.empty() || str[0] == '#')
				{
					get_str_before_char(config, "\n", &pos);
					continue ;
				}
				else if (str == "listen")
				{
					port = atoi(get_str_before_char(config, ";\n", &pos).c_str());
					DEBUG("\t" << port);
				}
				else if (str == "server_name")
				{
					std::string name;
					while ((name = get_str_before_char(config, " ;\n", &pos)).length())
					{
						DEBUG("\t" << name);
						server_names.push_back(name);
					}
				}
				else if (str == "root")
				{
					root = get_str_before_char(config, ";\n", &pos);
					DEBUG("\t" << root);
				}
				else if (str == "index")
				{
					index = get_str_before_char(config, ";\n", &pos);
					DEBUG("\t" << index);
				}
				else if (str == "error_pages")
				{
					std::string error_page;
					while ((error_page = get_str_before_char(config, " ;\n", &pos)).length())
						error_pages.push_back(error_page);
				}
				else if (str == "max_client_body_size")
				{
					max_client_body_size = atoi(get_str_before_char(config, ";\n", &pos).c_str());
					DEBUG("\t" << max_client_body_size);
				}
				else if (str == "location")
				{
					// Locations:
					std::string				location;
					std::list<std::string>	HTTP_methods;
					std::string				HTTP_redirection;
					std::string				location_root;
					bool 					directory_listing;
					std::string				default_answer;

					location = get_str_before_char(config, " ", &pos);
					DEBUG(location<<"\n{");
					if (get_str_before_char(config, " ;\n", &pos) != "{")
						continue ;
					std::string str_location;
					while ((str_location = get_str_before_char(config, " ;\n", &pos)) != "}")
					{
						DEBUG("\t"<<str_location<<":");
						if (str_location.empty() || str_location[0] == '#')
						{
							get_str_before_char(config, "\n", &pos);
							continue ;
						}
						else if (str_location == "HTTP_methods")
						{
							std::string HTTP_method;
							while ((HTTP_method = get_str_before_char(config, " ;", &pos)).length())
							{
								DEBUG("\t\t" << HTTP_method);
								HTTP_methods.push_back(HTTP_method);
							}
						}
						else if (str_location == "HTTP_redirection")
						{
							HTTP_redirection = get_str_before_char(config, ";\n", &pos);
							DEBUG("\t\t" << HTTP_redirection);
						}
						else if (str_location == "location_root")
						{
							location_root = get_str_before_char(config, ";\n", &pos);
							DEBUG("\t\t" << location_root);
						}
						else if (str_location == "directory_listing")
						{
							directory_listing = atoi(get_str_before_char(config, ";\n", &pos).c_str());
							DEBUG("\t\t" << directory_listing);
						}
						else if (str_location == "default_answer")
						{
							default_answer = get_str_before_char(config, ";\n", &pos);
							DEBUG("\t\t" << default_answer);
						}
						else
						{
							DEBUG("\t********************************OTHER_LOCATION: " << str_location);
							str_location = get_str_before_char(config, ";\n", &pos);
						}
					}
					DEBUG("}");
				}
				else
				{
					DEBUG("\t********************************OTHER: " << str);
					str = get_str_before_char(config, ";\n", &pos);
					// continue;
				}
			}
			DEBUG("END:" << str);
			servers.push_back(Server(server_names, error_pages, port, root, index, max_client_body_size));
		}
	}
	DEBUG("\n******** PARSED ********");
	DEBUG("");
}

void	Webserv::listen()
{
	while (true)
	{
		for (std::list<Server>::iterator server = servers.begin(); server != servers.end(); server++)
		{
			// DEBUG("---" << &server);
			server->s_listen();
		}
	}
}
