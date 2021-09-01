/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 13:31:42 by hthomas           #+#    #+#             */
/*   Updated: 2021/09/01 16:25:46 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

using namespace std;


string get_content_file(string filename)
{
    ifstream myfile;
    myfile.open(filename);
    string content("");
    while (myfile)
        content = content + (char)myfile.get();
    myfile.close();
    return content.substr(0, content.length() - 1);
}

int main()
{
        int server_fd, new_socket, valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        string buffer("", 1024);

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        // Forcefully attaching socket to the port 8080
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR /*| SO_REUSEPORT*/, &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

    while (1)
    {
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr*) &address, (socklen_t*)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        valread = read(new_socket, &buffer[0], 1024);
        buffer = buffer.substr(0, valread - 1);
        std::cout << buffer << std::endl;

        // int nfds = poll(struct pollfd fds[], nfds_t nfds, int timeout);

        string response("");
        if (!strncmp(&buffer[0], "GET / HTTP/1.1", sizeof("GET / HTTP/1.1") - 1))
        {
            response = response + "HTTP/1.1 200 OK\nContent-Length: 207\nContent-Type: text/html\nConnection: Closed\n\n";
            response = response + get_content_file("website/index.html");
        }
        else if (!strncmp(&buffer[0], "GET /hello.html HTTP/1.1", sizeof("GET /hello.html HTTP/1.1") - 1))
        {
            response = response + "HTTP/1.1 200 OK\nContent-Length: 207\nContent-Type: text/html\nConnection: Closed\n\n";
            response = response + get_content_file("website/hello.html");
        }
        else
            response = response + "HTTP/1.1 404 Not Found\nContent-Length: 119\nConnection: Closed\nContent-Type: text/html;\n\n<html lang=\"en\">\n<body>\n<h1>404 Not Found</h1>\n<p>The requested URL was not found on this server.</p>\n</body>\n</html>";
        send(new_socket, &response[0], response.length(), 0);
        std::cout << endl << "Response sent:" << endl << response << std::endl;
    }

    return 0;
}
