/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 13:31:42 by hthomas           #+#    #+#             */
/*   Updated: 2021/09/13 22:39:56 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "webserv.hpp"
#include "server.hpp"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

int main()
{
   Server serv;
   // serv.start_listen_socket();
   serv.setup();
   // while (true)
   // {
   serv.run();
   // serv.s_listen();
   // }
   return 0;
}
