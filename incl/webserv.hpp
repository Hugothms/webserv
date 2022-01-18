/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#             */
/*   Updated: 2022/01/18 11:56:02 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "includes.hpp"
// # include "server.hpp"
# include "client.hpp"

class Server;

class Webserv
{
	private:
		list<Server*>	_servers;
		list<Client*>	_clients;

		typedef struct
		{
			int 			fd;
			string 			ip_address;
			unsigned int 	port;	
		} fd_combo;


		list<fd_combo>		fd_list;
		// list<string>	fd_list;

		fd_set 			listen_set;
		fd_set 			lcopy_set;

		fd_set 			write_set;
		fd_set 			wcopy_set;

		int 			high_fd;

	public:
		Webserv(const string config_file = "");
		~Webserv();
		void	push_back_server(Server *server);
		bool	conflict_ip_address_port_server_names(const string &ip_address, const unsigned int port, const list<string> &server_names) const;
		void	parse_config(const string config);

		void	listen(void);
		void	loop_prep(void);
		void 	accept_new_conn(void);
		static void sig(void);
		void 	stop(void);
		void 	clear_fd(Client *client);
		void 	build(void);
		void 	process(Client *client);
};

#endif
