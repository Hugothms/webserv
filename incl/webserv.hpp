/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/27 14:21:49 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "includes.hpp"
# include "server.hpp"
# include "client.hpp"

class Server;

class Webserv
{
	private:
		list<Server*>	_servers;
		list<Client*>	_clients;

		fd_set 			listen_set;
		fd_set 			lcopy_set;

		fd_set 			write_set;
		fd_set 			wcopy_set;

		int 			high_fd;

	public:
		Webserv(const string config_file = "");
		~Webserv();
		void	listen();
		void 	accept_new_conn(void);
		static void sig();
		void 	stop();
		void 	build(void);
		void 	process(Client *client);
		bool	is_a_valid_server(	const list<Location>	locations,
									const list<string>		server_names,
									const map<int, string>	error_pages,
									const string			ip_address,
									const unsigned int 		port,
									const string			root,
									const string			index,
									const unsigned int		max_client_body_size);
		bool	conflict_ip_address_port_server_names(const string ip_address, const unsigned int port, const list<string> server_names);
};

#endif
