/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:29:50 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/12 14:05:38 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
#define INCLUDES_HPP

//STD INCLUDES
# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <cstdlib>
# include <unistd.h>
# include <ctime>
//NETWORK INCLUDES
# include <netinet/in.h>

// DATASTRUCTURE INCLUDES
# include <vector>
# include <list>
# include <map>

// using namespace std;

bool		isInteger(const std::string &s);
std::string	get_content_file(const std::string filename);
std::string	get_str_before_char(const std::string str, const std::string c, size_t *pos, const std::string skip = "\t 	");

#endif
