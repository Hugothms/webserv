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

class Location
{
	private :
		// * PARSED FROM CONFIG FILE *
		string			location;
		list<string>	HTTP_methods;
		string			HTTP_redirection;
		string			location_root;
		string			default_answer;
		string			index; // file served when a directory is requested
		bool 			directory_listing;

	public:
		Location();
		~Location();

		list<string>	get_HTTP_methods();
		string			get_location();
		string			get_HTTP_redirection();
		string			get_location_root();
		string			get_default_answer();
		string			get_index();
		bool			get_directory_listing();

		void set_HTTP_methods(const list<string> HTTP_methods);
		void push_back_HTTP_method(const string HTTP_method);
		void set_location(const string location);
		void set_HTTP_redirection(const string HTTP_redirection);
		void set_location_root(const string location_root);
		void set_default_answer(const string default_answer);
		void set_index(const string index);
		void set_directory_listing(const bool directory_listing);
};

#endif
