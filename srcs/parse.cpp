/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 16:55:59 by hthomas           #+#    #+#             */
/*   Updated: 2021/11/22 10:27:26 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Location	parse_location(const string &config, size_t *pos, Server *server)
{
	Location	location;
	string		tmp;
	tmp = get_str_before_char(config, " ", pos);
	if (tmp.length() > 0 && tmp[0] != '/')
		tmp = '/' + tmp;
	if (tmp.length() > 1 && tmp[tmp.size()] == '/')
		tmp.resize(tmp.length() - 1);
	DEBUG("\t" << tmp);
	DEBUG("\t{");
	location.set_path(tmp);
	if (get_str_before_char(config, " ;\n", pos) != "{")
		err_parsing_config(server, "expecting '{' after 'server'");
	while (config[*pos] && (tmp = get_str_before_char(config, " ;\n", pos)) != "}")
	{
		if (tmp[0] == '#')
		{
			// if (config[*pos-1] != '\n')
			// 	get_str_before_char(config, "\n", pos);
		}
		else if (tmp.length())
			DEBUG("\t\t" << tmp << ":");
		if (tmp == "allow")
		{
			while ((tmp = get_str_before_char(config, " ;", pos)).length())
			{
				DEBUG("\t\t\t" << tmp);
				location.push_back_HTTP_method(tmp);
			}
			get_str_before_char(config, "\n", pos);
		}
		else if (tmp == "return")
		{
			if ((tmp = get_str_before_char(config, " =;", pos)).length())
			{
				if (location.get_HTTP_redirection_type() != 0)
					err_parsing_config(server, "only one redirection can be set for a given location");
				int type = atoi(tmp.c_str());
				if (!(type == 300 || type == 301 || type == 302 || type == 303 || type == 304 || type == 307 || type == 308))
					err_parsing_config(server, "redirection code is invalid");
				if (config[*pos - 1] != ' ')
					err_parsing_config(server, "redirection not well configured");
				if ((tmp = get_str_before_char(config, ";", pos)).length())
				{
					location.set_HTTP_redirection_type(type);
					location.set_HTTP_redirection(tmp);
					DEBUG("\t\t" << type << " " << tmp);
				}
			}
			get_str_before_char(config, "\n", pos);
		}
		else if (tmp == "root")
		{
			if ((tmp = get_str_before_char(config, ";", pos)).length())
			{
				if (tmp.length() > 0 && tmp[0] != '/')
					tmp = '/' + tmp;
				if (tmp.length() > 1 && tmp[tmp.size()] == '/')
					tmp.resize(tmp.length() - 1);
				location.set_location_root(tmp);
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
			if ((tmp = get_str_before_char(config, ";", pos, "\r\t /")).length())
			{
				if (tmp.length() > 0 && tmp[0] != '/')
					tmp = '/' + tmp;
				if (tmp.length() > 1 && tmp[tmp.size()] == '/')
					tmp.resize(tmp.length() - 1);
				location.set_upload_directory(tmp);
				DEBUG("\t\t\t" << tmp);
				get_str_before_char(config, "\n", pos);
			}
		}
		else if (tmp == "autoindex")
		{
			if ((tmp = get_str_before_char(config, ";", pos)) == "0" || tmp == "1")
			{
				location.set_autoindex(atoi(tmp.c_str()));
				DEBUG("\t\t\t" << tmp);
				get_str_before_char(config, "\n", pos);
			}
		}
		else if (tmp.length())
		{
			// DEBUG("\t\t***OTHER_LOCATION: " << tmp);
			get_str_before_char(config, ";\n", pos);
		}
		// else
		// 	break;
	}
	if (tmp != "}")
	{
		cerr << location.get_path() << " location: no ending bracket" << endl;
		exit(EXIT_FAILURE);
	}
	DEBUG("\t}");
	if (location.get_location_root().length() == 0)
		location.set_location_root(location.get_path());
	location.set_server(server);
	return location;
}

Server	*parse_server(const string &config, size_t *pos)
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
		{
			if (config[*pos - 1] == ' ' || config[*pos - 1] == ';')
				(get_str_before_char(config, "\n", pos));
			else if (!config[*pos])
				break;
		}
		else if (tmp == "location")
			server->push_back_location(parse_location(config, pos, server));
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
				unsigned int error = atoi(tmp.c_str());
				if (config[*pos] != '=')
					err_parsing_config(server, "error page not well configured");
				(*pos)++;
				if ((tmp = get_str_before_char(config, ";", pos)).length())
				{
					server->push_back_error_page(pair<unsigned int, string>(error, tmp));
					DEBUG("\t\t" << error << " = " << tmp);
				}
			}
			get_str_before_char(config, "\n", pos);
		}
		else if (tmp == "cgi")
		{
			if ((tmp = get_str_before_char(config, " =;", pos)).length())
			{
				string extention = tmp;
				if (config[*pos - 1] != ' ')
					err_parsing_config(server, "cgi not well configured");
				if ((tmp = get_str_before_char(config, ";", pos)).length())
				{
					server->push_back_cgi(extention, tmp);
					DEBUG("\t\t" << extention << " " << tmp);
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
			if ((tmp = get_str_before_char(config, ";", pos)).length())
			{
				if (tmp.length() > 0 && tmp[0] == '/')
					tmp = tmp.substr(1);
				if (tmp.length() > 1 && tmp[tmp.size()] == '/')
					tmp.resize(tmp.length() - 1);
				server->set_root(tmp);
				get_str_before_char(config, "\n", pos);
			}
			DEBUG("\t\t" << server->get_root());
		}
		else if (tmp == "index")
		{
			if ((tmp = get_str_before_char(config, ";", pos)).length())
			{
				server->set_index(tmp);
				get_str_before_char(config, "\n", pos);
			}
			DEBUG("\t\t" << server->get_index());
		}
		else if (tmp == "max_client_body_size")
		{
			if (is_integer(tmp = get_str_before_char(config, ";", pos)))
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
		err_parsing_config(server, "no closing bracket");
	string error;
	if (!server->is_valid(error))
		err_parsing_config(server, error);
	return server;
}
