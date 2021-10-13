/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:47:21 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/13 15:53:34 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "includes.hpp"
# include "location.hpp"
# include "request.hpp"
# include "client.hpp"
# include "webserv.hpp"

# define V4 AF_INET
# define V6 AF_INET6
# define TCP SOCK_STREAM
# define UDP SOCK_DGRAM
# define IP 0
# define MAX_BACKLOG 10
# define MAX_CLIENTS 10
# define BUFFER_SIZE 4096

class Server
{
	private :
		// * PARSED FROM CONFIG FILE *
		 // port to listen and send on ("listen" in config file)
		 // "Host" header in HTTP request (domain names)
		
		

		// * FOR "INTERNAL" USE *
		// list<Request>		requests;
		bool 				_setup;

		// Enzo's attributes:
		int 				high_fd;
		fd_set				master_set;
		fd_set				copy_set;
		vector<Client>		_clients;

		
		unsigned int		port;
		int 				listen_fd;
		struct sockaddr_in	hint;

		list<string>		server_names;
		string				root; // directory where the webste is
		string				index; // file served when "/" is requested
		list<string>		error_pages; //
		unsigned int		max_client_body_size;
		list<Location>		locations;

	public:
		Server(	list<Location>	locations,
				list<string>	server_names,
				list<string>	error_pages,
				unsigned int	port = 4242,
				string			root = "website",
				string			index = "index.html",
				unsigned int	max_client_body_size = 2048);
		~Server();
		unsigned int			get_port();
		void s_listen();

 		Client handle_new_conn();
		int setup();
		int run(void);
		// Client handle_new_conn(int listen_sock);
		// int port;
};

#endif
