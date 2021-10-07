/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 13:38:54 by hthomas           #+#    #+#             */
/*   Updated: 2021/10/07 14:39:43 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

std::string get_content_file(std::string filename)
{
	std::ifstream myfile;
	myfile.open(filename);
	std::string content("");
	while (myfile)
		content = content + (char)myfile.get();
	myfile.close();
	return content.substr(0, content.length() - 1);
}

std::string get_str_before_char(std::string str, std::string stop, size_t *index, std::string skip)
{
	while (skip.find(str[*index]) == 0)
		(*index)++;
	size_t length = str.find(stop, *index) - *index;
	std::string res;
	if (length == std::string::npos || str.find('\n', *index) - *index < length)
		return res;
	res = str.substr(*index , length);
	*index += length + 1;
	return res;
}
