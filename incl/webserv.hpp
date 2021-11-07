/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#             */
/*   Updated: 2021/11/07 21:21:28 by edal--ce         ###   ########.fr       */
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
		void	loop_prep();
		void 	accept_new_conn(void);
		static void sig();
		void 	stop();
		void 	build(void);
		bool	is_a_valid_server(	const list<Location>	locations,
									const list<string>		server_names,
									const map<int, string>	error_pages,
									const string			host,
									const unsigned int 		port,
									const string			root,
									const string			index,
									const unsigned int		max_client_body_size);
		bool	conflict_host_port_server_names(const string host, const unsigned int port, const list<string> server_names);

};

#endif
