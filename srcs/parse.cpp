/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 16:55:59 by hthomas           #+#    #+#             */
/*   Updated: 2021/11/05 17:28:50 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	err_parsing_config(const string error)
{
	cerr << "Error: Wrong server configuration: " << error << endl;
	exit(5);
}

Location	parse_location(const string config, size_t *pos)
{
	Location	location;
	string		tmp;
	tmp = get_str_before_char(config, " ", pos);
	if (tmp.length() > 1 && tmp.back() == '/')
		tmp.resize(tmp.length() - 1);
	DEBUG("\t" << tmp << "\n\t{");
	location.set_path(tmp);
	if (get_str_before_char(config, " ;\n", pos) != "{")
		err_parsing_config("expecting '{' after 'server'");
	while (config[*pos] && (tmp = get_str_before_char(config, " ;\n", pos)) != "}")
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
		else if (tmp == "root")
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
		else if (tmp == "index")
		{
			if ((tmp = get_str_before_char(config, ";", pos)).length())
			{
				location.set_index(tmp);
				DEBUG("\t\t\t" << tmp);
				get_str_before_char(config, "\n", pos);
			}
		}
		else if (tmp == "upload_directory")
		{
			if ((tmp = get_str_before_char(config, ";", pos)).length())
			{
				location.set_upload_directory(tmp);
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
		else
			break;
	}
	if (tmp != "}")
	{
		DEBUG("Server configuration is invalid !");
		exit(EXIT_FAILURE);
	}
	DEBUG("\t}");
	return location;
}

Server	*parse_server(const string config, size_t *pos)
{
	string tmp;
	DEBUG("!!!!!!!!!! SERVER !!!!!!!!!!!");
	tmp = get_str_before_char(config, "\n", pos);
	if (tmp != "{")
		return NULL ;
	DEBUG("{");
	Server *server = new Server();
	while (config[*pos] && (tmp = get_str_before_char(config, " ;\n", pos)) != "}")
	{
		DEBUG("\t" << tmp);
		if (tmp.empty() || (tmp[0] == '#'))
		{if (config[*pos - 1] == ' ' || config[*pos - 1] == ';')
				(get_str_before_char(config, "\n", pos));
			else if (!config[*pos])
				break;
		}
		else if (tmp == "location")
			server->push_back_location(parse_location(config, pos));
		else if (tmp == "server_name")
		{
			while ((tmp = get_str_before_char(config, " ;", pos)).length())
			{
				DEBUG("\t\t" << tmp);
				server->push_back_server_name(tmp);
			}
			get_str_before_char(config, "\n", pos);
		}
		else if (tmp == "error_page")
		{
			if ((tmp = get_str_before_char(config, " =;", pos)).length())
			{
				int error = atoi(tmp.c_str());
				if (config[*pos] != '=')
					err_parsing_config("error page not well configured");
				(*pos)++;
				if ((tmp = get_str_before_char(config, ";", pos)).length())
				{
					server->push_back_error_page(pair<int, string>(error, tmp));
					DEBUG("\t\t" << error << " = " << tmp);
				}
			}
			get_str_before_char(config, "\n", pos);
		}
		else if (tmp == "listen")
		{
			if (!(tmp = get_str_before_char(config, ":", pos)).length())
				tmp = "0.0.0.0";
			if (tmp == "localhost")
				tmp = "127.0.0.1";
			server->set_ip_address(tmp);
			DEBUG("\t\tip_address: " << tmp);
			if (is_integer(tmp = get_str_before_char(config, ";", pos)))
			{
				server->set_port(atoi(tmp.c_str()));
				DEBUG("\t\tport: " << atoi(tmp.c_str()));
			}
			get_str_before_char(config, "\n", pos);
		}
		else if (tmp == "root")
		{
			if((tmp = get_str_before_char(config, ";", pos)).length())
			{
				server->set_root(tmp);
				get_str_before_char(config, "\n", pos);
			}
			DEBUG("\t\t" << server->get_root());
		}
		else if (tmp == "index")
		{
			if((tmp = get_str_before_char(config, ";", pos)).length())
			{
				server->set_index(tmp);
				get_str_before_char(config, "\n", pos);
			}
			DEBUG("\t\t" << server->get_index());
		}
		else if (tmp == "max_client_body_size")
		{
			if(is_integer(tmp = get_str_before_char(config, ";", pos)))
			{
				get_str_before_char(config, "\n", pos);
				server->set_max_client_body_size(atoi(tmp.c_str()));
			}
			DEBUG("\t\t" << server->get_max_client_body_size());
		}
		else if (tmp.length())
		{
			DEBUG("\t\t***OTHER: " << tmp);
			tmp = get_str_before_char(config, "\n", pos);
		}
	}
	if (tmp != "}")
	{
		DEBUG("Server configuration is invalid !");
		exit(EXIT_FAILURE);
	}
	DEBUG("}");
	if (!server->is_valid())
	{
		DEBUG("Server configuration is invalid !");
		exit(EXIT_FAILURE);
	}
	return server;
}
