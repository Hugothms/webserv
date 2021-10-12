/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/12 21:08:05 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "includes.hpp"
# include "server.hpp"

# if DEBUG_ACTIVE == 1
#  define DEBUG(x) cout << x << endl;
# else
#  define DEBUG(x)
# endif

class Server;

class Webserv
{
	private:
		list<Server>	servers;
	public:
		Webserv(string config_file = "");
		void	listen();
};

#endif
