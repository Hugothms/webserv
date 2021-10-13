/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:47:21 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/13 17:19:56 by hthomas          ###   ########.fr       */
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
		list<Location>		locations;
		list<string>		server_names; // "Host" header in HTTP request (domain names)
		list<string>		error_pages; //
		string				host;
		unsigned int		port; // port to listen and send on ("listen" in config file)
		string				root; // directory where the webste is
		string				index; // file served when "/" is requested
		unsigned int		max_client_body_size;

		// * FOR "INTERNAL" USE *
		// list<Request>		requests;
		bool 				_setup;
		int 				high_fd;
		fd_set				copy_set;

		vector<Client>		_clients;
		fd_set				master_set;


		int 				listen_fd;
		struct sockaddr_in	hint;

	public:
		Server();
		Server(	list<Location>	locations,
				list<string>	server_names,
				list<string>	error_pages,
				string			host,
				unsigned int	port,
				string			root,
				string			index,
				unsigned int	max_client_body_size);
		~Server();
		void s_listen();

 		Client handle_new_conn();
		int setup();
		int run(void);
		// Client handle_new_conn(int listen_sock);
		// int port;

		unsigned int	get_port();

		void 			set_locations(list<Location> locations);
		void 			push_back_location(Location location);
		void 			set_server_names(list<string> server_names);
		void 			push_back_server_name(string server_name);
		void 			set_error_pages(list<string> error_pages);
		void 			push_back_error_page(string error_page);
		void 			set_host(string host);
		void 			set_port(unsigned int port);
		void 			set_root(string root);
		void 			set_index(string index);
		void 			set_max_client_body_size(unsigned int max_client_body_size);
};

#endif
