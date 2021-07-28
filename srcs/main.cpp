/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 13:31:42 by hthomas           #+#    #+#             */
/*   Updated: 2021/07/28 16:27:31 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "webserv.hpp"
#include "server.hpp"
// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

using namespace std;

int main()
{

    Server serv;
    while (true)
       serv.s_listen();
    return 0;
}
