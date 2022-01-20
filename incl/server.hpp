/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:47:21 by edal--ce          #+#    #+#             */
/*   Updated: 2022/01/20 17:53:58 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SERVER_HPP
# define SERVER_HPP

# include "includes.hpp"
// # include "location.hpp"
# include "request.hpp"
// # include "client.hpp"

// class Request;

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
		list<Location>				locations;
		list<string>				server_names; // "Host" header in HTTP request (domain names)
		map<string, string>			cgis; // CGI's extentions names and exec paths
		map<unsigned int, string>	error_pages; //
		string						root; // directory where the webste is
		string						index; // file served when "/" is requested
		string						ip_address;
		unsigned int				port; // port to listen and send on ("listen" in config file)
		long long					max_client_body_size;

		// * FOR INTERNAL USE *
		int						listen_fd;
		struct sockaddr_in			hint;

	public:
		Server();
		~Server();
		void						s_listen(void);
		int							setup(void);

		bool						is_valid(string &error) const;
		list<Location>				get_locations(void) const;
		list<string>				get_server_names(void) const;
		map<string, string>			get_cgis(void) const;
		map<unsigned int, string>	get_error_pages(void) const;
		string						get_ip_address(void) const;
		unsigned int				get_port(void) const;
		string						get_root(void) const;
		string						get_index(void) const;
		long long					get_max_client_body_size(void) const;
		int							get_listen_fd(void) const;

		void						set_locations(const list<Location> locations);
		void						push_back_location(const Location location);
		void						set_server_names(const list<string> server_names);
		void						push_back_server_name(const string server_name);
		void						set_cgis(const map<string, string> cgis);
		void						push_back_cgi(const string extention_name, const string exec_path);
		void						set_error_pages(const map<unsigned int, string> error_pages);
		void						push_back_error_page(const pair<unsigned int, string> error_page);
		void						set_ip_address(const string ip_address);
		void						set_port(const unsigned int port);
		void						set_root(const string root);
		void						set_index(const string index);
		void						set_max_client_body_size(const long long max_client_body_size);
};

#endif
