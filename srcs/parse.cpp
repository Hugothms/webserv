/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 16:55:59 by hthomas           #+#    #+#             */
/*   Updated: 2022/01/11 15:37:05 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Location	parse_location(const vector<string> &config, size_t *line_count, Server *server)
{
	Location	location;
	const vector<string>	line = ft_split(config, WHITESPACES);

	if (line.length() != 3)
		err_parsing_config(server, "expecting a directory and '{' after 'location'");
	if (line[1].length() > 0 && line[1][0] != '/')
		line[1] = '/' + line[1];
	if (line[1].length() > 1 && line[1][line[1].size() - 1] == '/')
		line[1].resize(line[1].length() - 1);
	DEBUG("\t" << line[1]);
	DEBUG("\t{");
	location.set_path(line[1]);
	if (line[3] != "{")
		err_parsing_config(server, "expecting '{' after 'location' directory");
	for (vector<string>::const_iterator it = config.begin(); it != config.end(); it++)
	{
		vector<string> line = ft_split(*it, WHITESPACES);
		DEBUG("\t" << line[0]);
		if (line[0][0] == '#')
		{
			break;
			// if (config[*line_count-1] != '\n')
		}
		else if (line[0].length())
			DEBUG("\t\t" << line[0] << ":");
		if (line[0] == "allow")
		{
			for (vector<string>::const_iterator allow_m = line.begin(); allow_m != line.end(); allow_m++)
			{
				DEBUG("\t\t\t" << allow_m);
				location.push_back_HTTP_method(allow_m);
			}
		}
		else if (line[0] == "return")
		{
			if (line.length() == 3)
			{
				if (location.get_HTTP_redirection_type() != 0)
					err_parsing_config(server, "only one redirection can be set for a given location");
				int type = atoi(line[0].c_str());
				if (!(type == 300 || type == 301 || type == 302 || type == 303 || type == 304 || type == 307 || type == 308))
					err_parsing_config(server, "redirection code is invalid");
				if (config[*line_count - 1] != ' ')
					err_parsing_config(server, "redirection not well configured");
				if (line[1].length())
				{
					location.set_HTTP_redirection_type(type);
					location.set_HTTP_redirection(line[0]);
					DEBUG("\t\t" << type << " " << line[0]);
				}
			}
		}
		else if (line[0] == "root")
		{
			if (line.length() != 2)
				err_parsing_config(server, "expecting a directory and '{' after 'location'");
				//todo: unindent all
				if (line[0].length() > 0 && line[0][0] != '/')
					line[0] = '/' + line[0];
				if (line[0].length() > 1 && line[0][line[0].size() - 1] == '/')
					line[0].resize(line[0].length() - 1);
				location.set_location_root(line[0]);
				DEBUG("\t\t\t" << line[0]);
		}
		else if (line[0] == "index")
		{
			if (line[1].length())
			{
				location.set_index(line[0]);
				DEBUG("\t\t\t" << line[0]);
			}
		}
		else if (line[0] == "autoindex")
		{
			if ((line[0] = get_str_before_char(config, ";", line_count)) == "0" || line[0] == "1")
			{
				location.set_autoindex(atoi(line[0].c_str()));
				DEBUG("\t\t\t" << line[0]);
			}
		}
		else if (line[0].length())
		{
			// DEBUG("\t\t***OTHER_LOCATION: " << line[0]);
			get_str_before_char(config, ";\n", line_count);
		}
		// else
		// 	break;
	}
	if (line[0] != "}" && config[end] != '}' && *line_count != end)
	{
		cerr << location.get_path() << " location: no closing bracket" << endl;
		exit(EXIT_FAILURE);
	}
	DEBUG("\t}");
	if (location.get_location_root().length() == 0)
		location.set_location_root(location.get_path());
	location.set_server(server);
	return location;
}

Server	*parse_server(const vector<string> config, size_t *line_count)
{
	DEBUG("!!!!!!!!!! SERVER !!!!!!!!!!!");

	vector<string> line = ft_split(config[0], " ");;
	if (line[0] != "{")
		return NULL;
	DEBUG("{");
	Server *server = new Server();
	for (vector<string>::const_iterator it = config.begin(); it != config.end(); it++)
	{
		vector<string> line = ft_split(*it, " ");
		DEBUG("\t" << line[0]);
		// if (line[0].empty() || (line[0][0] == '#'))
		// {
		// 	if (config[*line_count - 1] == ' ' || config[*line_count - 1] == ';')
		// 		(get_str_before_char(config, "\n", line_count));
		// 	else if (!config[*line_count])
		// 		break;
		// 	else
		// 		break;
		// }
		if (line[0] == "location")
			server->push_back_location(parse_location(config, line_count, server));
		else if (line[0] == "server_name")
		{
			if (line.length() <= 2)
				err_parsing_config(server, "expecting at least 1 argument after 'server_name'");
			for (vector<string>::const_iterator s_name = line.begin()++; s_name != line.end(); s_name++)
				server->push_back_server_name(*s_name);
		}
		else if (line[0] == "error_page")
		{
			if (line.length() != 2)
				err_parsing_config(server, "expecting 2 arguments after 'error_page'");
			server->push_back_error_page(pair<unsigned int, string>(atoi(line[0].c_str()), line[1]));
			DEBUG("\t\t" << atoi(line[0].c_str()) << " = " << line[1]);
		}
		else if (line[0] == "cgi")
		{
			if (line.length() != 3)
				err_parsing_config(server, "expecting 2 arguments after 'cgi'");
			server->push_back_cgi(line[1], line[2]);
			DEBUG("\t\t" << line[1] << " " << line[2]);
		}
		else if (line[0] == "listen")
		{
			if (line.length() != 2)
				err_parsing_config(server, "expecting 1 arguments after 'listen'");
			vector<string> listen = ft_split(line[1], ":");
			if (listen.length() == 1)
				listen.push_front("0.0.0.0");
			if (listen[0] == "localhost")
				listen[0] = "127.0.0.1";
			server->set_ip_address(listen[0]);
			DEBUG("\t\tip_address: " << listen[0]);
			if (is_integer(listen[1]))
			{
				server->set_port(atoi(listen[1].c_str()));
				DEBUG("\t\tport: " << atoi(listen[1].c_str()));
			}
			else
				err_parsing_config(server, "listen not well configured");
		}
		else if (line[0] == "root")
		{
			if (line.length() != 2)
				err_parsing_config(server, "expecting 1 argument after 'root'");
			string tmp = line[1];
			if (tmp.length() > 0 && tmp[0] == '/')
				tmp = tmp.substr(1);
			if (tmp.length() > 1 && tmp[tmp.size() - 1] == '/')
				tmp.resize(tmp.length() - 1);
			server->set_root(tmp);
			}
			DEBUG("\t\t" << server->get_root());
		}
		else if (line[0] == "index")
		{
			if (line.length() != 2)
				err_parsing_config(server, "expecting 1 argument after 'index'");
			server->set_index(line[0]);
			DEBUG("\t\t" << server->get_index());
		}
		else if (line[0] == "max_client_body_size")
		{
			if (line.length() != 2)
				err_parsing_config(server, "expecting 1 arguments after 'max_client_body_size'");
			if (!is_integer(line[1]))
				err_parsing_config(server, "expecting positive integer after 'max_client_body_size'");
			server->set_max_client_body_size(atoi(line[1].c_str()));
			DEBUG("\t\t" << server->get_max_client_body_size());
		}
		else if (line.length())
		{
			DEBUG("\t\t***OTHER: " << line[0]);
		}
	}
	// line_count = config[];
	DEBUG("\n\nline[0]         :" << line[0]);
	// DEBUG("config[end] |" << config[end] << "|" )
	DEBUG("line_count         :" << *line_count);
	// if (config[end] == '}' && *line_count >= end)
	// 	err_parsing_config(server, "no empty line after closing bracket");
	// if ((line[0] == "location" && (config[end] == '}' || *line_count >= end)) ||
	// 	(!line[0].length() && *line_count >= end))
	// 	err_parsing_config(server, "no closing bracket");
	string error;
	if (!server->is_valid(error))
		err_parsing_config(server, error);
	return server;
}
