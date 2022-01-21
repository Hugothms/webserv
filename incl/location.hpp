/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:47:21 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/08 14:23:10 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "includes.hpp"

class Server;

class Location
{
	private :
		// * PARSED FROM CONFIG FILE *
		Server			*server;
		string			path;
		list<string>	allow;
		unsigned int	HTTP_redirection_type;
		string			HTTP_redirection;
		string			location_root;
		string			index;
		bool			autoindex;

	public:
		Location();
		~Location();
		bool			is_valid(string &error) const;

		Server			*get_server(void) const;
		list<string>	get_allow(void) const;
		string			get_path(void) const;
		unsigned int	get_HTTP_redirection_type(void) const;
		string			get_HTTP_redirection(void) const;
		string			get_root(void) const;
		string			get_index(void) const;
		bool			get_autoindex(void) const;

		void			set_server(Server *server);
		void			set_allow(const list<string> allow);
		void			push_back_HTTP_method(const string HTTP_method);
		void			set_path(const string location);
		void			set_HTTP_redirection_type(const unsigned int type);
		void			set_HTTP_redirection(const string HTTP_redirection);
		void			set_root(const string location_root);
		void			set_index(const string index);
		void			set_autoindex(const bool autoindex);
};

#endif
