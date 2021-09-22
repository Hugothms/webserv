/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 14:24:05 by edal--ce          #+#    #+#             */
/*   Updated: 2021/09/22 13:53:32 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "includes.hpp"

class Request
{
	private :
		std::string		type;			// GET or POST or DELETE
		std::string		target;			// The URL requested
		int				socket;			// ? i don't know
		// std::map<std::string, std::string>	headers;	// jsp si je prefere peut etre une map de headers (a tous les headers en dessous) -> a voir later
		std::string		host;			// Name of the server (here localhost)
		int				port;			// specified in the request (host:port)
		std::string		user_agent;		// Info about the platform
		std::string		accept;			// List of accepted stuff
		std::string		accept_language;
		std::string		accept_encoding;
		std::string		connection;
		std::string		cache_control;
		std::string		content_type;	// Only for POST
		unsigned int	content_length;	// Only for POST

		std::string		body;			// Only for POST

		/* Ignored because not HTTP/1.1 compliant
		std::string		dnt;
		std::string		upgrade_insecure_requests;
		std::string		sec_fetch_dest;
		std::string		sec_fetch_mode;
		std::string		sec_fetch_site;
		std::string		sec_fetch_user;
		std::string		sec_gpc;
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
		Request();
		Request(char *buffer, size_t size, int socket=80);
		~Request();
		void respond(void);
};

#endif
