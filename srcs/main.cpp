/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#             */
/*   Updated: 2022/01/19 15:51:09 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "server.hpp"
#include <signal.h>

Webserv* ptr;

void stop(int num)
{
	(void) num;
	DEBUG("stop: KILLING");
	ptr->stop();
	DEBUG("stop: KILLED");
	exit(0);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cerr << "Error: exactly one argument is required (config file path)" << endl;
		return -1;
	}
	else
	{
		ifstream file(argv[1]);
		if (!file.is_open())
		{
			cerr << "Error: File does not exist" << endl;
			return -1;
		}
		file.close();
	}
	Webserv webserv((argc == 1) ? "" : argv[1]);

	ptr = &webserv;
	signal(SIGPIPE, SIG_IGN);
	// signal(SIGSTOP, &stop);
	signal(SIGINT, &stop);
	signal(SIGKILL, &stop);

	webserv.listen();
	return 0;
}
