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
		list<string>	HTTP_methods;
		string			HTTP_redirection;
		string			location_root;
		string			index; // file served when a directory is requested
		string			upload_directory;
		bool 			directory_listing;

	public:
		Location();
		~Location();
		string			is_valid(void) const;

		Server			*get_server() const;
		list<string>	get_HTTP_methods() const;
		string			get_path() const;
		string			get_HTTP_redirection() const;
		string			get_location_root() const;
		string			get_index() const;
		string			get_upload_directory() const;
		bool			get_directory_listing() const;

		void set_server(Server *server);
		void set_HTTP_methods(const list<string> HTTP_methods);
		void push_back_HTTP_method(const string HTTP_method);
		void set_path(const string location);
		void set_HTTP_redirection(const string HTTP_redirection);
		void set_location_root(const string location_root);
		void set_index(const string index);
		void set_upload_directory(const string index);
		void set_directory_listing(const bool directory_listing);


		// <string, void (Location::*)(const string)>

		// typedef void (Location::*my_pointer_function)(const string);
		// map<string, my_pointer_function const> map_pointer_function;

		// void set(const string &key, const string value) {
		// 	my_pointer_function fp = map_pointer_function[key];
		// 	return (this->*fp)(value);
		// }
};

#endif
