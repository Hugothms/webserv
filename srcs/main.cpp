/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#             */
/*   Updated: 2021/12/24 17:31:17 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "server.hpp"
#include <signal.h>

Webserv* ptr;

void stop(int num)
{
	(void) num;
	// DEBUG("KILLING");
	ptr->stop();
	// DEBUG("KILLED");
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
