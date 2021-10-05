/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 14:24:05 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/05 16:55:26 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <map>
# include "includes.hpp"

class Request
{
	private :
		std::string		type;			// GET or POST or DELETE
		std::string		target;			// The URL requested
		int				socket;			// ? i don't know
		std::map<std::string, std::string>	headers;
		// std::string		body;			// Only for POST

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
