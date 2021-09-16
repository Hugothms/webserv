/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 14:24:05 by edal--ce          #+#    #+#             */
/*   Updated: 2021/09/16 11:05:37 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "includes.hpp"

class Request
{
	private :
		std::string type;		//GET OR POST
		std::string target;		//THE URL REQUESTED
		std::string user_agent;	//Info about the platform
		std::string accept;		//List of accepted stuff
		int			socket;

	public :
		Request();
		Request(char *buffer, int size, int socket);
		~Request();
		void respond(void);
};

#endif
