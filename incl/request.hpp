/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 14:24:05 by edal--ce          #+#    #+#             */
/*   Updated: 2021/08/17 16:43:59 by edal--ce         ###   ########.fr       */
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
		int 		socket;
		// const char *hello = "HTTP/1.1 200 OK\nContent-Length: 52\nContent-Type: text/html\nConnection: Closed\n\n<html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>\n";
	public :
		Request();
		Request(char *buffer, int size, int socket);
		~Request();
		void respond(void);
};
 
#endif