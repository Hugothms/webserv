/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:47:21 by edal--ce          #+#    #+#             */
/*   Updated: 2021/11/17 12:02:14 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
# define SERVER_HPP

# include "includes.hpp"
# include "location.hpp"
# include "request.hpp"
// # include "client.hpp"

# define V4 AF_INET
# define V6 AF_INET6
# define TCP SOCK_STREAM
# define UDP SOCK_DGRAM
# define IP 0
# define MAX_BACKLOG 10
# define MAX_CLIENTS 10
# define BUFFER_SIZE 4096

class Location;

class Server
{
	private :
		// * PARSED FROM CONFIG FILE *
		list<Location>		locations;
		list<string>		server_names; // "Host" header in HTTP request (domain names)
		list<string>		cgis; // CGI's extentions name
		map<int, string>	error_pages; //
		string				ip_address;
		unsigned int		port; // port to listen and send on ("listen" in config file)
		string				root; // directory where the webste is
		string				index; // file served when "/" is requested
		unsigned int		max_client_body_size;

		// * FOR INTERNAL USE *
		int 				listen_fd;
		struct sockaddr_in	hint;

	public:
		Server();
		~Server();
		void	s_listen();
		int		setup();

		list<Location>		get_locations() const;
		list<string>		get_server_names() const;
		list<string>		get_cgis() const;
		map<int, string>	get_error_pages() const;
		string				get_ip_address() const;
		unsigned int		get_port() const;
		string				get_root() const;
		string				get_index() const;
		unsigned int		get_max_client_body_size() const;
		int 				get_listen_fd(void) const;
		string				is_valid(void) const;

		void set_locations(const list<Location> locations);
		void push_back_location(const Location location);
		void set_server_names(const list<string> server_names);
		void push_back_server_name(const string server_name);
		void set_cgis(const list<string> cgi);
		void push_back_cgi(const string cgi);
		void set_error_pages(const map<int, string> error_pages);
		void push_back_error_page(const pair<int, string> error_page);
		void set_ip_address(const string ip_address);
		void set_port(const unsigned int port);
		void set_root(const string root);
		void set_index(const string index);
		void set_max_client_body_size(const unsigned int max_client_body_size);
};

#endif
