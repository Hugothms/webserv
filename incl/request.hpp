/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 14:24:05 by edal--ce          #+#    #+#             */
/*   Updated: 2022/01/19 22:24:53 by edal--ce         ###   ########.fr       */
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

		// * FOR INTERNAL USE *
		const Server		*server;
		const Location		*location;
		string				filepath;
		string				content_type;
		bool				passed_cgi;
		unsigned int		code;

		/* Ignored because not HTTP/1.1 compliant
		string		dnt;
		string		upgrade_insecure_requests;
		string		sec_fetch_dest;
		string		sec_fetch_mode;
		string		sec_fetch_site;
		string		sec_fetch_user;
		string		sec_gpc;
		*/

		/* Unimplemented headers:
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

		char **build_cgi_env(string &extention_name);
		char **build_cgi_av(string &extention_name);

	public :
		Request(const string &buffer);
		~Request();
		void	prep_response(const list<Server*> &servers);
		string	get_s_header(string name);
		void	add_to_body(string toadd);
		string	respond(const list<Server*> &servers, char fast_pipe = 0);
		bool	select_server(const list<Server*> &servers);
		bool	select_location(void);
		string	get_response(void);
		void	get_body(string &body);
		string	get_cgi_header(const size_t fileSize);
		string	get_header(size_t fileSize, const bool already_calculated);
		void	launch_cgi(string &body, const int pos);
		void	get_auto_index(string &body);
		void	set_filepath(void);
		string	get_filepath(void);
		bool	method_allow(void);
		string	error_page(const int error_code);
		int	get_file_status(int &nfd);
		void	delete_rq(void);
		bool	body_size_ok(unsigned int size);

		int		get_max_body() const;
		string	get_type(void) const;
		int		get_code(void) const;
		string& give_head(string header);

};

// * utils_header.cpp
string	getdayofweek(const int day);
string	getmonth(const int month);
string	get_time_stamp(void);
string	get_type(const string &str, const bool &pass);

#endif
