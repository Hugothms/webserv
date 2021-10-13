/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/13 13:23:56 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

Location::~Location() {}

Location::Location() {}

Location::Location(	string			location,
					list<string>	HTTP_methods,
					string			HTTP_redirection,
					string			location_root,
					bool 			directory_listing,
					string			default_answer)
	: location(location), HTTP_methods(HTTP_methods), HTTP_redirection(HTTP_redirection), location_root(location_root), directory_listing(directory_listing), default_answer(default_answer)
{}
