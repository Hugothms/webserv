/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/19 18:27:21 by edal--ce         ###   ########.fr       */
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

		fd_set 			master_set;
		fd_set 			copy_set;
		int 			high_fd;

	public:
		Webserv(string config_file = "");
		~Webserv();
		void	listen();
		static void sig();
		void 	stop();
		void 	build(void);
		void 	process(Client *client);
		bool	is_a_valid_server(	list<Location>	locations,
							list<string>	server_names,
							list<string>	error_pages,
							string			host,
							unsigned int 	port,
							string			root,
							string			index,
							unsigned int	max_client_body_size);
		bool	conflict_host_port_server_names(string host, unsigned int port, list<string> server_names);

};

#endif
