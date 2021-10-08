/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/08 17:31:48 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

Location::~Location() {}

Location::Location(	std::string				location,
					std::list<std::string>	HTTP_methods,
					std::string				HTTP_redirection,
					std::string				location_root,
					bool 					directory_listing,
					std::string				default_answer)
	: location(location), HTTP_methods(HTTP_methods), HTTP_redirection(HTTP_redirection), location_root(location_root), directory_listing(directory_listing), default_answer(default_answer)
{
}
