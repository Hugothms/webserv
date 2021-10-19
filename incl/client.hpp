/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 23:06:00 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/19 18:40:13 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "includes.hpp"
# include "server.hpp"


class Client
{
	private :
	public :
		int					fd;
		struct sockaddr_in	client_addr;
		char				client_ipv4_str[INET_ADDRSTRLEN];
		socklen_t			client_len;
		Server 				*server;
		Client()
		{
			// memset(&client_addr, 0, sizeof(client_addr));
			fd = 0;
			client_len = sizeof(client_addr);
			server = 0;
		}
		void set_fd(int nfd)
		{
			fd = nfd;
		}
		int get_fd(void)
		{
			return fd;
		}
		~Client()
		{
			DEBUG("KILLING CLIENT\n");
			if (fd > 0)
			{
				close(fd);
				DEBUG("KILLED\n");
			}
		}
		void set_server(Server *s)
		{
			server = s;
		}
		struct sockaddr* get_sockaddr(void)
		{
			return (struct sockaddr*)(&client_addr);
		}
		socklen_t *get_addr_len(void)
		{
			return (&client_len);
		}
		char *v4str(void)
		{
			return client_ipv4_str;
		}
		void identify(void)
		{
			cout << client_ipv4_str <<":" <<client_addr.sin_port << endl << endl;
		}
};

#endif
