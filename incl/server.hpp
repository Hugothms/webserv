/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:47:21 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/12 23:12:45 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "includes.hpp"
#include "location.hpp"
#include "request.hpp"
#include "client.hpp"
#include "webserv.hpp"

// using namespace std;
#define V4 AF_INET
#define V6 AF_INET6
#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM
#define IP 0
#define MAX_BACKLOG 10
#define MAX_CLIENTS 10
#define BUFFER_SIZE 4096
// #define DEBUG 1

class Server
{
	private :

	public:
		Server(	list<Location>		locations,
				list<string>	server_names,
				list<string>	error_pages,
				unsigned int			port = 8080,
				string				root = "website",
				string				index = "index.html",
				unsigned int			max_client_body_size = 2048);
		~Server();
		void s_listen();

 	
	Client handle_new_conn();
	int setup();
	int run(void);
	// Client handle_new_conn(int listen_sock);
		private :
		// * PARSED FROM CONFIG FILE *
		// unsigned int				port; // port to listen and send on ("listen" in config file)
		list<string>		server_names; // "Host" header in HTTP request (domain names)
		string					root; // directory where the webste is
		string					index; // file served when "/" is requested
		list<string>		error_pages; //
		unsigned int				max_client_body_size;
		list<Location>			locations;

		// * FOR "INTERNAL" USE *
		list<Request>			requests;
		bool 					_setup;



	int listen_fd;
	int port;
	struct sockaddr_in hint;
	fd_set master_set;
	fd_set copy_set;
	vector<Client> _clients;
};

#endif
