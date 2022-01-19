/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 13:38:54 by hthomas           #+#    #+#             */
/*   Updated: 2022/01/19 10:32:46 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "webserv.hpp"

bool is_integer(const string &s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
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
	return content;
}

static bool	ft_in_charset(char const c, const string &charset)
{
	int	i_charset;

	i_charset = 0;
	while (charset[i_charset])
	{
		if (c == charset[i_charset++])
			return true;
	}
	return false;
}

/**
 * split str into a char ** each time there is a char from charset
 * @param str		"sentence" that will be splited
 * @param charset	set of "delimiters" that will delimits the "words"
 * @return			a tab of "words"
**/
vector<string> ft_split(const string &str, const string &charset)
{
	vector<string> res;
	string			tmp;
	size_t			i;

	i = 0;
	while (i < str.length())
	{
		while (i < str.length() && ft_in_charset(str[i], charset))
			i++;
		if (i < str.length())
		{
			tmp = "";
			while (i < str.length() && !ft_in_charset(str[i], charset))
				tmp += str[i++];
			res.push_back(tmp);
		}
	}
	return (res);
}

/**
 * @param str	string where to extract the substr
 * @param stop	set of stoping chars
 * @param pos	position where to start in str
 * @param skip	set of chars to ignore at the begin (default: " \\t")
 * @return first substring found between pos and the first occurence of a char in 'stop' (or empty string if '\\n' is found first)
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

bool is_directory(const string &filename)
{
	struct stat st_buf;
	int status = stat(filename.c_str(), &st_buf);
	if (status != 0) {
		return false;
	}
	return (S_ISDIR(st_buf.st_mode) == 1);
}

void	err_parsing_config(const Server *server, const string error)
{
	if (server)
	{
		cerr << RED << "'" << server->get_root() << "' server configuration is invalid: " << error << '.' << RESET << endl;
		delete server;
	}
	else
		cerr << RED << "configuration is invalid." << RESET << endl;
	exit(EXIT_FAILURE);
}
