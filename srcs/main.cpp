/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/07 12:02:58 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "server.hpp"
// #include <unistd.h>
// #include <cstdlib>
// #include <cstring>
// #include <sys/socket.h>
// #include <netinet/in.h>

std::string get_content_file(std::string filename)
{
	std::ifstream myfile;
	myfile.open(filename);
	std::string content("");
	while (myfile)
		content = content + (char)myfile.get();
	myfile.close();
	return content.substr(0, content.length() - 1);
}

int main()
{
	Server serv;
	while (true)
		serv.s_listen();
	return 0;
}
