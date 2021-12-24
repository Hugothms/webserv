/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 14:24:05 by edal--ce          #+#    #+#             */
/*   Updated: 2021/12/24 16:02:19 by edal--ce         ###   ########.fr       */
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
		

		// * FOR INTERNAL USE *
		const Server		*server;
		const Location		*location;
		string				filepath;
		unsigned int		code;
		bool 				passed_cgi;


		map<string, string>	headers;
		string 				content_type;

		char **build_cgi_env(string &extention_name);
		char **build_cgi_av(string &extention_name);
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
		string &get_s_header(string name);
		void add_to_body(string toadd);
		Request(const string &buffer);
		~Request();
		string	respond(const list<Server*> &servers);
		bool	select_server(const list<Server*> &servers);
		bool	select_location(void);
		string 	g_type(void) const;
		string 	get_response(void);
		void 	get_body(string &body);
		string	get_header(const size_t length);
		void 	launch_cgi(string &body, const string extention_name);
		void	get_auto_index(string &body);
		void	set_filepath(void);
		bool 	method_allow(void);
		string	error_page(const int error_code);
};

// * utils_header.cpp
string	getdayofweek(const int day);
string	getmonth(const int month);
string	get_time_stamp(void);
string	get_type(const string &str, const bool &pass);

#endif
