/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 13:38:54 by hthomas           #+#    #+#             */
/*   Updated: 2021/11/15 17:04:12 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "webserv.hpp"

bool is_integer(const string &s)
{
	if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
		return false;
	char *p;
	strtol(s.c_str(), &p, 10);
	return (*p == 0);
}

string get_content_file(const string &filename)
{
	ifstream myfile(filename.c_str());

	string content((istreambuf_iterator<char>(myfile)), istreambuf_iterator<char>());
	myfile.close();
	// return content.substr(0, content.length() - 1);
	return content;
}

/**
 * @param str	string where to extract the substr
 * @param stop	set of stoping chars
 * @param pos	position where to start in str
 * @param skip	set of chars to ignore at the begin (default: " \\t")
 * @return first substring found between the start and the first occurence of a char in 'stop' (or empty string if '\\n' is found first)
 *
**/
string get_str_before_char(const string &str, const string stop, size_t *pos, const string skip)
{
	while (skip.find(str[*pos]) < skip.length())
		(*pos)++;
	size_t length = string::npos;
	for (size_t i = 0; i < stop.length(); i++)
	{
		if (str.find(stop[i], *pos) - *pos < length)
			length = str.find(stop[i], *pos) - *pos;
	}
	if (*pos + length == string::npos || (str.find('\n', *pos) - *pos < length))
		return "";
	string res;
	if (str[*pos + length - 1] == '\r')
		res = str.substr(*pos, length - 1);
	res = str.substr(*pos, length);
	*pos += length + 1;
	return res;
}

bool file_is_empty(ifstream &file)
{
	return (file.peek() == ifstream::traits_type::eof());
}
