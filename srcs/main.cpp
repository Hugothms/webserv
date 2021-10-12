/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/12 20:49:34 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "server.hpp"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
// #define PORT 8080
int main(int argc, char *argv[])
{
	if (argc != 1 && argc != 2)
	{
		std::cerr << "ERROR: Wrong number of arguments" << std::endl;
		return -1;
	}
	if (argc == 2)
	{
		std::ifstream file(argv[1]);
		if(!file.is_open())
		{
			std::cerr << "ERROR: File does not exist" << std::endl;
			return -1;
		}
		file.close();
	}
	Webserv webserv((argc == 1) ? "" : argv[1]);
	webserv.listen();
	return 0;
}
