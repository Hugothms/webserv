/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:29:50 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/12 21:08:33 by hthomas          ###   ########.fr       */
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

using namespace std;

# include "location.hpp"

class Location;

bool		is_a_valid_server(	list<Location>		locations,
								list<string>	server_names,
								list<string>	error_pages,
								unsigned int 			port,
								string				root,
								string				index,
								unsigned int			max_client_body_size);
bool		is_integer(const string &s);
string	get_content_file(const string filename);
string	get_str_before_char(const string str, const string c, size_t *pos, const string skip = "\t 	");
//NEW

#include <arpa/inet.h>
#include <sys/select.h>


#endif
