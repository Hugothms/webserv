/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:29:50 by edal--ce          #+#    #+#             */
/*   Updated: 2021/11/08 12:28:46 by hthomas          ###   ########.fr       */
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
# include <sys/socket.h> // bind

// DATASTRUCTURE INCLUDES
# include <vector>
# include <list>
# include <map>

using namespace std;

// OTHERS INCLUDES
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


bool	is_integer(const string &s);
string	get_content_file(const string filename);
string	get_str_before_char(const string str, const string stop, size_t *pos, const string skip = "\r\t ");

// * PARSING *
void	err_parsing_config(const string error);
Server	*parse_server(const string config, size_t *pos);


# define code_100 "100 Continue"
# define code_101 "101 Switching Protocols"
# define code_200 "200 OK"
# define code_201 "201 Created"
# define code_202 "202 Accepted"
# define code_203 "203 Non-Authoritative Information"
# define code_204 "204 No Content"
# define code_205 "205 Reset Content"
# define code_300 "300 Multiple Choices"
# define code_301 "301 Moved Permanently"
# define code_302 "302 Found"
# define code_303 "303 See Other"
# define code_305 "305 Use Proxy"
# define code_306 "306 (Unused)"
# define code_307 "307 Temporary Redirect"
# define code_400 "400 Bad Request"
# define code_402 "402 Payment Required"
# define code_403 "403 Forbidden"
# define code_404 "404 Not Found"
# define code_405 "405 Method Not Allowed"
# define code_406 "406 Not Acceptable"
# define code_408 "408 Request Timeout"
# define code_409 "409 Conflict"
# define code_410 "410 Gone"
# define code_411 "411 Length Required"
# define code_413 "413 Payload Too Large"
# define code_414 "414 URI Too Long"
# define code_415 "415 Unsupported Media Type"
# define code_417 "417 Expectation Failed"
# define code_426 "426 Upgrade Required"
# define code_500 "500 Internal Server Error"
# define code_501 "501 Not Implemented"
# define code_502 "502 Bad Gateway"
# define code_503 "503 Service Unavailable"
# define code_504 "504 Gateway Timeout"
# define code_505 "505 HTTP Version Not Supported"

#endif
