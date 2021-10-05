#include <iostream>
// #include <WinSock2.h>
// #pragma comment(lib, "Ws2_32.lib")
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
	int sock;
	struct sockaddr_in server;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(6666);

	if (sock == -1)
	{
		std::cout << "Erreur creation socket : " << std::endl;
	 	return 0;
	}
	int reuse = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
		std::cout << "setsockopt\n";
		return -1;
	}

	if (bind(sock, (struct sockaddr*)&server, sizeof(server)) != 0)
	{
		std::cout << "Erreur connection : " << std::endl;
		return 0;
	}
	std::cout << "Socket connecte !" << std::endl;
	close(sock);
	// closesocket(socket);
	// WSACleanup();
 }