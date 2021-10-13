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
		bool 			directory_listing;
		string			default_answer;

	public:
		Location();
		Location(	string			location,
					list<string>	HTTP_methods,
					string			HTTP_redirection,
					string			location_root,
					bool 			directory_listing,
					string			default_answer);
		~Location();
};

#endif
