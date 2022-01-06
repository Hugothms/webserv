/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:29:50 by edal--ce          #+#    #+#             */
/*   Updated: 2021/12/28 18:57:43 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
# define INCLUDES_HPP

//* STD INCLUDES
# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <cstdlib>
# include <unistd.h>
# include <ctime>
# include <sys/stat.h> // mkdir
# include <dirent.h> // dirent readdir
# include <sys/types.h>
# include <sys/wait.h>

//* NETWORK INCLUDES
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <sys/socket.h> // bind

//* DATASTRUCTURE INCLUDES
# include <vector>
# include <list>
# include <map>

using namespace std;

//* OUR INCLUDES
# include "location.hpp"
# include "server.hpp"
# include "log.hpp"

# if DEBUG_ACTIVE == 1
#  define DEBUG(x) cerr << x << endl;
# else
#  define DEBUG(x)
# endif

#define DEBUG_LINE(x) cerr << __FILE__ << ':' << __LINE__ << '\t' << x << endl

class Server;
class Location;


//* PARSING
Server	*parse_server(const string &config, size_t *pos);
bool	is_integer(const string &s);
string	get_content_file(const string &filename);
string	get_str_before_char(const string &str, const string stop, size_t *pos, const string skip = "\r\t ");
bool	file_is_empty(ifstream &file);
bool	is_directory(const string &filename);
void	err_parsing_config(const Server *server, const string error);

#endif
