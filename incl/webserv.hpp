/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/13 15:40:13 by hthomas          ###   ########.fr       */
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
		list<Server>	servers;
	public:
		Webserv(string config_file = "");
		void	listen();
		bool	is_a_valid_server(	list<Location>	locations,
							list<string>	server_names,
							list<string>	error_pages,
							unsigned int 	port,
							string			root,
							string			index,
							unsigned int	max_client_body_size);
};

#endif
