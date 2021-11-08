/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#             */
/*   Updated: 2021/11/08 12:23:48 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "server.hpp"
#include <signal.h>

Webserv* ptr;

void stop(int num)
{
	(void) num;
	DEBUG("KILLING");
	ptr->stop();
	DEBUG("KILLED");
	exit(0);
}

int main(int argc, char *argv[])
{
	if (argc != 1 && argc != 2)
	{
		cerr << "ERROR: Wrong number of arguments" << endl;
		return -1;
	}
	if (argc == 2)
	{
		ifstream file(argv[1]);
		if(!file.is_open())
		{
			cerr << "ERROR: File does not exist" << endl;
			return -1;
		}
		file.close();
	}
	Webserv webserv((argc == 1) ? "" : argv[1]);
	
	ptr = &webserv;
	signal(SIGPIPE, SIG_IGN);
	signal(SIGSTOP, &stop);
	signal(SIGINT, &stop);
	signal(SIGKILL, &stop);

	webserv.listen();
	return 0;
}
