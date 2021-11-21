/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 14:24:05 by edal--ce          #+#    #+#             */
/*   Updated: 2021/11/21 17:36:23 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "includes.hpp"
# include "server.hpp"

class Server;
class Location;

class Request
{
	private :
		// * PARSED FROM HTTP REQUEST *
		string				type;			// GET or POST or DELETE
		string				target;			// The URL requested
		map<string, string>	headers;
		// string		body;			// Only for POST

		// * FOR INTERNAL USE *
		const Server		*server;
		const Location		*location;
		string				filepath;
		string				message;

		/* Ignored because not HTTP/1.1 compliant
		string		dnt;
		string		upgrade_insecure_requests;
		string		sec_fetch_dest;
		string		sec_fetch_mode;
		string		sec_fetch_site;
		string		sec_fetch_user;
		string		sec_gpc;
		*/

		/* Unimplemented headers for now:
		Accept-Charset
		Authorization
		Expect
		From
		If-Match
		If-Modified-Since
		If-None-Match
		If-Range
		If-Unmodified-Since
		Max-Forwards
		Proxy-Authorization
		Range
		Referer
		TE
		*/

	public :
		Request(const string &buffer);
		~Request();
		string	respond(const list<Server*> &servers);
		bool	select_server(const list<Server*> &servers);
		bool	select_location(void);
		string 	get_response(void);
		void 	get_body(string &body);
		string	get_header(const size_t length);
		void 	launch_cgi(string &body, const string extention_name);
		void	get_auto_index(string &body);
		void	set_filepath(void);
		bool 	method_allow(void);
		string	error_page(const int error_code);
		bool	is_file_upload(void);
};

// * utils_header.cpp
string	getdayofweek(const int day);
string	getmonth(const int month);
string	get_time_stamp(void);
string	get_type(const string &str);

map<unsigned int, string> create_map_return_codes(void)
{
	codes[100] = "100 Continue";
	codes[101] = "101 Switching Protocols";
	codes[200] = "200 OK";
	codes[201] = "201 Created";
	codes[202] = "202 Accepted";
	codes[203] = "203 Non-Authoritative Information";
	codes[204] = "204 No Content";
	codes[205] = "205 Reset Content";
	codes[300] = "300 Multiple Choices";
	codes[301] = "301 Moved Permanently";
	codes[302] = "302 Found";
	codes[303] = "303 See Other";
	codes[305] = "305 Use Proxy";
	codes[306] = "306 (Unused)";
	codes[307] = "307 Temporary Redirect";
	codes[400] = "400 Bad Request";
	codes[402] = "402 Payment Required";
	codes[403] = "403 Forbidden";
	codes[404] = "404 Not Found";
	codes[405] = "405 Method Not allow";
	codes[406] = "406 Not Acceptable";
	codes[408] = "408 Request Timeout";
	codes[409] = "409 Conflict";
	codes[410] = "410 Gone";
	codes[411] = "411 Length Required";
	codes[413] = "413 Payload Too Large";
	codes[414] = "414 URI Too Long";
	codes[415] = "415 Unsupported Media Type";
	codes[417] = "417 Expectation Failed";
	codes[426] = "426 Upgrade Required";
	codes[500] = "500 Internal Server Error";
	codes[501] = "501 Not Implemented";
	codes[502] = "502 Bad Gateway";
	codes[503] = "503 Service Unavailable";
	codes[504] = "504 Gateway Timeout";
	codes[505] = "505 HTTP Version Not Supported";
	return codes;
}

map<unsigned int, string> codes = create_map_return_codes();

#endif
