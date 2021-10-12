/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 13:38:54 by hthomas           #+#    #+#             */
/*   Updated: 2021/10/12 15:07:05 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "webserv.hpp"

bool is_integer(const std::string &s)
{
	if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
		return false;
	char *p;
	strtol(s.c_str(), &p, 10);
	return (*p == 0);
}

std::string get_content_file(const std::string filename)
{
	std::ifstream myfile;
	myfile.open(filename);
	std::string content("");
	while (myfile)
		content = content + (char)myfile.get();
	myfile.close();
	return content.substr(0, content.length() - 1);
}

/**
 * @param str	string where to extract the substr
 * @param stop	set of stoping chars
 * @param pos	position where to start in str
 * @param skip	set of chars to ignore at the begin (default: " \\t")
 * @return first substring found between the start and the first occurence of a char in 'stop' (or empty string if '\\n' is found first)
 *
**/
std::string get_str_before_char(const std::string str, const std::string stop, size_t *pos, const std::string skip)
{
	while (skip.find(str[*pos]) < skip.length())
		(*pos)++;
	size_t length = std::string::npos;
	for (size_t i = 0; i < stop.length(); i++)
	{
		if (str.find(stop[i], *pos) - *pos < length)
			length = str.find(stop[i], *pos) - *pos;
	}
	std::string res;
	if (length == std::string::npos || str.find('\n', *pos) < *pos + length)
		return res;
	res = str.substr(*pos , length);
	*pos += length + 1;
	return res;
}

bool	is_a_valid_server(	std::list<Location>		locations,
							std::list<std::string>	server_names,
							std::list<std::string>	error_pages,
							unsigned int 			port,
							std::string				root,
							std::string				index,
							unsigned int			max_client_body_size)
{
	// TODO
	return true;
}
