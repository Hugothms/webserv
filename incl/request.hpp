/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 14:24:05 by edal--ce          #+#    #+#             */
/*   Updated: 2021/08/03 16:30:25 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

// #include "server.hpp"


#include <vector>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 

#include <string>
#include <fstream>
#include <iostream>
// #include <.h>
class Request
{
	private :
		std::string type;		//GET OR POST
		std::string target;		//THE URL REQUESTED
		std::string user_agent;	//Info about the platform	
		std::string accept;		//List of accepted stuff
		int 		socket;

	public :
		Request();
		Request(char *buffer, int size, int socket);
		~Request();
		void respond(void);
};

#endif