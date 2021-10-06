/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 18:59:50 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/06 17:21:31 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#define PORT 4242

int main()
{
	//Create Socket
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if (listen_sock == -1)
	{
		std::cerr << "Socket error\n";
		return -1;
	}


	//Bind the socket to a IP / port
	struct sockaddr_in hint;
	
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	inet_pton(AF_INET, "0.0.0.0", &(hint.sin_addr));

	if (bind(listen_sock, (const struct sockaddr *)&hint, sizeof(hint)) == -1)
	{
		std::cerr << "Bind error\n";
		return -2;
	}

	listen(listen_sock, SOMAXCONN);

	fd_set master; 
	fd_set copy;

	//Add the listening new conn socket to the watched set
	FD_ZERO(&master);
	FD_SET(listen_sock, &master);
	
	int high_sock = listen_sock;
	while (1)
	{
		copy = master;

		int sock_count = select(high_sock + 1, &copy, nullptr, nullptr, nullptr);
		//The sockets will be stocked in copy
		if (FD_ISSET(listen_sock, &copy))
		{
			std::cout << "New conn incomming, need to accept it !\n";
			
			struct sockaddr_in client_addr;
			memset(&client_addr, 0, sizeof(client_addr));
			socklen_t client_len = sizeof(client_addr);
			
			int new_client = accept(listen_sock, (struct sockaddr *)&client_addr, &client_len);

			char client_ipv4_str[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &client_addr.sin_addr, client_ipv4_str, INET_ADDRSTRLEN);
			printf("Incoming connection from %s:%d.\n", client_ipv4_str, client_addr.sin_port);
			//Accept conn
		}

		// for (int i = 0; i < sock_count; i++)
		// {
		// 	//Accept conn
		// 	if (i == listen_sock)
		// 	//Receive message
		// }
	}

	// fd_set master;

	// FD_ZERO(&master);

	// FD_SET(listen_sock, &master);

	// while(true)
	// {
	// 	fd_set copy = master;

	// 	int socketCount = select(0, &copy, 0, 0, 0);

		// for(int i = 0; i < socketCount; i++)
		// {
		// 	int sock = copy.fd_array[i];
		// 	if (sock == listen_sock)
		// 	{
		// 		//Accept new conn
		// 		int client = accept(listen_sock, 0, 0);
		// 		//Add it to list of connected clients
		// 		FD_SET(client, &master);
		// 		//Send welcome
		// 		std::string test = "Hello world";
		// 		send(client, test.c_str(), test.size() + 1, 0);
		// 	}
		// 	else
		// 	{
		// 		char buff[4096];
		// 		memset(buff, 4096, 0);
		// 		int bytesIn = recv(sock, buf, 4096, 0);
		// 		if (bytesIn <= 0)
		// 		{
		// 			//Drop client
		// 			close(sock);
		// 			FD_CLR(sock, &master);
		// 		}
		// 		else
		// 		{
		// 			for (int i =0 ; i < master.fd_count; i++)
		// 			{
		// 				int outsock = master.fd_array[i];
		// 				if (outsock != listen_sock && outsock != sock)
		// 				{
		// 					send(outsock, buff, bytesIn, 0);
		// 				}
		// 			}	
		// 		}
		// 		//Accept message from established conn
		// 		//Send response to clients and not sockets
		// 	}
		// }
	// }


	// if ( == -1)
	// {
	// 	std::cerr << "can't listen\n";
	// 	return -3;
	// }

	// struct sockaddr_in client;
	// socklen_t client_size = sizeof(client);

	// char host[NI_MAXHOST];
	// char svc[NI_MAXSERV];

	// int clientsocket = accept(listen_sock, (sockaddr *)&client, &client_size);

	// if (clientsocket == -1)
	// {
	// 	std::cerr << "Accept error\n";
	// 	return -4;
	// }

	// close(listen_sock);

	// memset(host, 0, NI_MAXHOST);
	// memset(svc, 0, NI_MAXSERV);


	// int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV,0);
	
	// if (result)
	// {
	// 	std::cout <<host<< " connected to " << svc << std::endl;
	// }
	// else
	// {
	// 	inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
	// 	std::cout << host << " connected to " << ntohs(client.sin_port) << std::endl;
	// }

	// char buff[4096];
	// while(1)
	// {
	// 	memset(buff, 0, 4096);
	// 	int bytes_rcv = recv(clientsocket, buff, 4096, 0);
	// 	if (bytes_rcv == -1)
	// 	{
	// 		std::cerr << "Conn error\n";
	// 		break;
	// 	}
	// 	else if (bytes_rcv == 0)
	// 	{
	// 		std::cerr << "Client DC\n";
	// 		break;
	// 	}
	// 	std::cerr << "received: " ;
	// 	write(2,buff, bytes_rcv);//<< string(buff, 0, bytes_rcv) << std::endl;
	// 	write(2,"\n",1);

	// 	send(clientsocket, buff, bytes_rcv + 1, 0);
	// }

	// close(clientsocket);
	close(listen_sock);
	//Mark socket for listen
	//Accept a call
	//Close the listening socket
	//Print rcv msg
	//Close socket
	return 0;
}
// https://www.youtube.com/watch?v=dquxuXeZXgo