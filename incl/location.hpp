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
		std::string				location;
		std::list<std::string>	HTTP_methods;
		std::string				HTTP_redirection;
		std::string				location_root;
		bool 					directory_listing;
		std::string				default_answer;

	public:
		Location(	std::string				location,
					std::list<std::string>	HTTP_methods,
					std::string				HTTP_redirection,
					std::string				location_root,
					bool 					directory_listing,
					std::string				default_answer);
		~Location();
};

#endif
