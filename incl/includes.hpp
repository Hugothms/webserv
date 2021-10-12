/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:29:50 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/12 20:55:22 by hthomas          ###   ########.fr       */
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

# include "location.hpp"

class Location;

bool		is_a_valid_server(	std::list<Location>		locations,
								std::list<std::string>	server_names,
								std::list<std::string>	error_pages,
								unsigned int 			port,
								std::string				root,
								std::string				index,
								unsigned int			max_client_body_size);
bool		is_integer(const std::string &s);
std::string	get_content_file(const std::string filename);
std::string	get_str_before_char(const std::string str, const std::string c, size_t *pos, const std::string skip = "\t 	");
//NEW

#include <arpa/inet.h>
#include <sys/select.h>


#endif
