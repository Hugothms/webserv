/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/25 21:17:45 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "includes.hpp"
# include "server.hpp"

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
		static void sig();
		void 	stop();
		void 	build(void);
		void 	process(Client *client);
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
