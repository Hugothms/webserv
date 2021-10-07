/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 11:55:53 by hthomas           #+#    #+#             */
/*   Updated: 2021/10/07 14:37:25 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Webserv::Webserv(std::string config_file)
{

	if (config_file == "")
	{
		DEBUG("Default config (no config provided)");
		servers.push_back(Server(std::vector<std::string>(), std::vector<std::string>()));
	}
	// else
	// {
	// 	std::string config = get_content_file(config_file);
	// 	DEBUG("Provided config:\n----------------");
	// 	DEBUG(config);

	// 	// Parse and add multiple servers in "servers"
	// 	size_t index = 0;
	// 	std::string first_word;
	// 	while (config[index]) // config parsing loop
	// 	{
	// 		first_word = get_str_before_char(config, " ", &index);
	// 		if (first_word == "server")
	// 		{
	// 			if (get_str_before_char(config, " ", &index) != "{")
	// 			{
	// 				/* code */
	// 			}

	// 			while (first_word != "}")
	// 			{
	// 				/* code */
	// 			}

	// 		}




	// 		// To be parsed
	// 		unsigned int port;
	// 		std::list<std::string> names;
	// 		std::string name;

	// 		if (first_word == "\0")
	// 			break ; // case empty line
	// 		index++;
	// 		if (first_word == "server_name")
	// 		{
	// 			while ((name = get_str_before_char(config, " ", &index)).length())
	// 				names.push_back(name);
	// 			continue;
	// 		}
	// 		else if (first_word == "location")
	// 		{

	// 		}
	// 		qwerty.insert(std::pair<std::string, std::string>(first_word, get_str_before_char(config, "\n", &index)));
	// 		servers.push_back(Server());
	// 	}

	// 	DEBUG("\n******** PARSED ********");
	// 	std::map<std::string, std::string>::iterator it = qwerty.begin();
	// 	while(it != qwerty.end())
	// 		DEBUG(it->first << ": " << it++->second);
	// 	DEBUG("");
	// }
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
