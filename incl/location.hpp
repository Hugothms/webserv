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
#include "server.hpp"

class Server;

class Location
{
	private :
		// * PARSED FROM CONFIG FILE *
		Server			*server;
		string			path;
		list<string>	allow;
		string			HTTP_redirection;
		string			location_root;
		string			index; // file served when a directory is requested
		string			upload_directory;
		bool 			autoindex;

	public:
		Location();
		~Location();
		bool			is_valid(string &error) const;
		Server			*get_server(void) const;
		list<string>	get_allow(void) const;
		string			get_path(void) const;
		string			get_HTTP_redirection(void) const;
		string			get_location_root(void) const;
		string			get_index(void) const;
		string			get_upload_directory(void) const;
		bool			get_autoindex(void) const;

		void set_server(Server *server);
		void set_allow(const list<string> allow);
		void push_back_HTTP_method(const string HTTP_method);
		void set_path(const string location);
		void set_HTTP_redirection(const string HTTP_redirection);
		void set_location_root(const string location_root);
		void set_index(const string index);
		void set_upload_directory(const string index);
		void set_autoindex(const bool autoindex);


		// <string, void (Location::*)(const string)>

		// typedef void (Location::*my_pointer_function)(const string);
		// map<string, my_pointer_function const> map_pointer_function;

		// void set(const string &key, const string value) {
		// 	my_pointer_function fp = map_pointer_function[key];
		// 	return (this->*fp)(value);
		// }
};

#endif
