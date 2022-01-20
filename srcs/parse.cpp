/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 16:55:59 by hthomas           #+#    #+#             */
/*   Updated: 2022/01/20 16:30:27 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

Location	parse_location(const vector<string> &config, size_t *line_count, Server *server)
{
	Location	location;
	vector<string> line = ft_split(config[*line_count], WHITESPACES);

	if (line.size() != 3)
		err_parsing_config(server, "expecting a directory and '{' after 'location'");
	string tmp = line[1];
	if (tmp.size() > 0 && tmp[0] != '/')
		tmp = '/' + tmp;
	if (tmp.size() > 1 && tmp[tmp.size() - 1] == '/')
		tmp.resize(tmp.size() - 1);
	location.set_path(tmp);
	DEBUG("\t" << location.get_path());
	if (line[2] != "{")
		err_parsing_config(server, "expecting '{' after 'location' directory");
	(*line_count)++;
	DEBUG("\t{");
	vector<string>::const_iterator it = config.begin() + *line_count;
	while (it != config.end())
	{
		vector<string> line = ft_split(*it, WHITESPACES);
		if (line[0][0] == '#')
		{
			it++;
			(*line_count)++;
			continue;
		}
		else if (line[0].size() && line[0] != "}")
			DEBUG("\t\t" << line[0] << ":");

		if (line[0] == "}")
			break;
		else if (line[0] == "allow")
		{
			if (line.size() < 2)
				err_parsing_config(server, "expecting at least 1 argument after 'allow'");
			for (vector<string>::const_iterator allow_m = line.begin() + 1; allow_m != line.end(); allow_m++)
			{
				DEBUG("\t\t\t" << *allow_m);
				location.push_back_HTTP_method(*allow_m);
			}
		}
		else if (line[0] == "return")
		{
			if (line.size() != 3)
				err_parsing_config(server, "expecting 2 arguments after 'return'");
			if (location.get_HTTP_redirection_type() != 0)
				err_parsing_config(server, "only one redirection can be set for a given location");
			int type = atoi(line[1].c_str());
			if (!(type == 300 || type == 301 || type == 302 || type == 303 || type == 304 || type == 307 || type == 308))
				err_parsing_config(server, "redirection code is invalid");
			if (line[1].size())
			{
				location.set_HTTP_redirection_type(type);
				location.set_HTTP_redirection(line[2]);
				DEBUG("\t\t\t" << type << " " << line[2]);
			}
		}
		else if (line[0] == "root")
		{
			if (line.size() != 2)
				err_parsing_config(server, "expecting 1 argument after 'root'");
			string tmp = line[1];
			if (tmp.size() > 0 && tmp[0] != '/')
				tmp = '/' + tmp;
			if (tmp.size() > 1 && tmp[tmp.size() - 1] == '/')
				tmp.resize(tmp.size() - 1);
			location.set_location_root(tmp);
			DEBUG("\t\t\t" << tmp);
		}
		else if (line[0] == "index")
		{
			if (line.size() != 2)
				err_parsing_config(server, "expecting 1 argument after 'index'");
			location.set_index(line[1]);
			DEBUG("\t\t\t" << line[1]);
		}
		else if (line[0] == "autoindex")
		{
			if (line.size() != 2)
				err_parsing_config(server, "expecting 1 argument after 'autoindex'");
			if (line[1] == "0" || line[1] == "1")
			{
				location.set_autoindex(atoi(line[1].c_str()));
				DEBUG("\t\t\t" << location.get_autoindex());
			}
		}
		else
		{
			DEBUG("\t\t***OTHER LOCATION: " << line[0]);
		}
		it++;
		(*line_count)++;
	}
	vector<string> end = ft_split(config[*line_count], WHITESPACES);
	if (end[0] != "}")
	{
		cerr << location.get_path() << " location: missing closing bracket" << endl;
		exit(EXIT_FAILURE);
	}
	DEBUG("\t}");
	if (location.get_location_root().size() == 0)
		location.set_location_root(location.get_path());
	location.set_server(server);
	return location;
}

Server	*parse_server(const vector<string> config, size_t *line_count)
{
	DEBUG("!!!!!!!!!! SERVER !!!!!!!!!!!");

	vector<string> line = ft_split(config[0], WHITESPACES);
	if (line[1] != "{")
		return NULL;
	DEBUG("{");
	Server *server = new Server();
	vector<string>::const_iterator it = config.begin() + *line_count;
	if (it == config.end())
		return NULL;
	it++;
	(*line_count)++;
	while (it != config.end())
	{
		vector<string> line = ft_split(*it, WHITESPACES);
		if (!line.size() || !line[0].size())
			break;
		if (line[0][0] == '#')
		{
			it++;
			(*line_count)++;
			continue;
		}
		else if (line[0] != "}")
			DEBUG("\t" << line[0] << ":");
		if (line[0] == "}")
		{
			// (*line_count)++;
			break;
		}
		else if (line[0] == "location")
		{
			if (line.size() != 3)
				err_parsing_config(server, "expecting 2 arguments after 'location'");
			size_t old_line_count = *line_count;
			server->push_back_location(parse_location(config, line_count, server));
			it += *line_count - old_line_count;
		}
		else if (line[0] == "server_name")
		{
			if (line.size() < 2)
				err_parsing_config(server, "expecting at least 1 argument after 'server_name'");
			for (vector<string>::const_iterator s_name = line.begin() + 1; s_name != line.end(); s_name++)
			{
				server->push_back_server_name(*s_name);
				DEBUG("\t\t" << *s_name);
			}
		}
		else if (line[0] == "error_page")
		{
			if (line.size() != 3)
				err_parsing_config(server, "expecting 2 arguments after 'error_page'");
			server->push_back_error_page(pair<unsigned int, string>((unsigned int)atoi(line[1].c_str()), line[2]));
			DEBUG("\t\t" << ((unsigned int)atoi(line[1].c_str())) << " " << line[2]);
		}
		else if (line[0] == "cgi")
		{
			if (line.size() != 3)
				err_parsing_config(server, "expecting 2 arguments after 'cgi'");
			server->push_back_cgi(line[1], line[2]);
			DEBUG("\t\t" << line[1] << " " << line[2]);
		}
		else if (line[0] == "listen")
		{
			if (line.size() != 2)
				err_parsing_config(server, "expecting 1 argument after 'listen'");
			vector<string> listen = ft_split(line[1], ":");
			if (listen.size() != 1 && listen.size() != 2)
				err_parsing_config(server, "expecting 'port' or 'address:port' after 'listen'");
			string address;
			string port;
			address = listen[0];
			if (listen.size() == 1)
			{
				address = "0.0.0.0";
				port = listen[0];
			}
			else
				port = listen[1];
			if (address == "localhost")
				address = "127.0.0.1";
			server->set_ip_address(address);
			DEBUG("\t\tip_address: " << address);
			if (!is_integer(port))
				err_parsing_config(server, "port in listen should be an integer");
			server->set_port(atoi(port.c_str()));
			DEBUG("\t\tport: " << atoi(port.c_str()));
		}
		else if (line[0] == "root")
		{
			if (line.size() != 2)
				err_parsing_config(server, "expecting 1 argument after 'root'");
			string tmp = line[1];
			if (tmp.size() > 0 && tmp[0] == '/')
				tmp = tmp.substr(1);
			if (tmp.size() > 1 && tmp[tmp.size() - 1] == '/')
				tmp.resize(tmp.size() - 1);
			server->set_root(tmp);
			DEBUG("\t\t" << server->get_root());
		}
		else if (line[0] == "index")
		{
			if (line.size() != 2)
				err_parsing_config(server, "expecting 1 argument after 'index'");
			server->set_index(line[1]);
			DEBUG("\t\t" << server->get_index());
		}
		else if (line[0] == "max_client_body_size")
		{
			if (line.size() != 2)
				err_parsing_config(server, "expecting 1 argument after 'max_client_body_size'");
			if (!is_integer(line[1]))
				err_parsing_config(server, "expecting positive integer after 'max_client_body_size'");
			server->set_max_client_body_size(atoi(line[1].c_str()));
			DEBUG("\t\t" << server->get_max_client_body_size());
		}
		else
		{
			DEBUG("\t\t***OTHER: " << line[0]);
		}
		it++;
		(*line_count)++;
	}
	vector<string> end = ft_split(*it, WHITESPACES);
	while (!end.size() || end[0] != "}")
	{
		(*line_count)++;
		end = ft_split(config[*line_count], WHITESPACES);
	}
	if (end.size() && end[0] != "}")
	{
		err_parsing_config(server, "missing closing bracket");
		exit(EXIT_FAILURE);
	}
	DEBUG("}");
	string error;
	if (!server->is_valid(error))
		err_parsing_config(server, error);
	return server;
}
