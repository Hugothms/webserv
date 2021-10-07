/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/07 13:16:10 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "server.hpp"
// #include <unistd.h>
// #include <cstdlib>
// #include <cstring>
// #include <sys/socket.h>
// #include <netinet/in.h>

int main(int argc, char *argv[])
{
	Webserv webserv((argc == 1) ? "" : argv[1]);
	webserv.listen();
	return 0;
}
