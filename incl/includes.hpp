/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:29:50 by edal--ce          #+#    #+#             */
/*   Updated: 2021/11/15 17:54:21 by hthomas          ###   ########.fr       */
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

# define CODE_100 "100 Continue"
# define CODE_101 "101 Switching Protocols"
# define CODE_200 "200 OK"
# define CODE_201 "201 Created"
# define CODE_202 "202 Accepted"
# define CODE_203 "203 Non-Authoritative Information"
# define CODE_204 "204 No Content"
# define CODE_205 "205 Reset Content"
# define CODE_300 "300 Multiple Choices"
# define CODE_301 "301 Moved Permanently"
# define CODE_302 "302 Found"
# define CODE_303 "303 See Other"
# define CODE_305 "305 Use Proxy"
# define CODE_306 "306 (Unused)"
# define CODE_307 "307 Temporary Redirect"
# define CODE_400 "400 Bad Request"
# define CODE_402 "402 Payment Required"
# define CODE_403 "403 Forbidden"
# define CODE_404 "404 Not Found"
# define CODE_405 "405 Method Not Allowed"
# define CODE_406 "406 Not Acceptable"
# define CODE_408 "408 Request Timeout"
# define CODE_409 "409 Conflict"
# define CODE_410 "410 Gone"
# define CODE_411 "411 Length Required"
# define CODE_413 "413 Payload Too Large"
# define CODE_414 "414 URI Too Long"
# define CODE_415 "415 Unsupported Media Type"
# define CODE_417 "417 Expectation Failed"
# define CODE_426 "426 Upgrade Required"
# define CODE_500 "500 Internal Server Error"
# define CODE_501 "501 Not Implemented"
# define CODE_502 "502 Bad Gateway"
# define CODE_503 "503 Service Unavailable"
# define CODE_504 "504 Gateway Timeout"
# define CODE_505 "505 HTTP Version Not Supported"

#endif
