/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:29:50 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/27 13:03:00 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
# define INCLUDES_HPP

// STD INCLUDES
# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <cstdlib>
# include <unistd.h>
# include <ctime>

// NETWORK INCLUDES
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/select.h>

// DATASTRUCTURE INCLUDES
# include <vector>
# include <list>
# include <map>

using namespace std;

// OTHERS INCLUDES
# include "location.hpp"


# if DEBUG_ACTIVE == 1
#  define DEBUG(x) cerr << x << endl;
# else
#  define DEBUG(x)
# endif

class Location;


bool	is_integer(const string &s);
string	get_content_file(const string filename);
string	get_str_before_char(const string str, const string c, size_t *pos, const string skip = "\r\t 	");

#endif
