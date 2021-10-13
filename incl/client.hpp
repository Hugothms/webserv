/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 23:06:00 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/13 19:31:17 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "includes.hpp"
# include "server.hpp"
class Client
{
	public :
		int					fd;
		struct sockaddr_in	client_addr;
		char				client_ipv4_str[INET_ADDRSTRLEN];
		socklen_t			client_len;
		// Server 				*srv;
		Client()
		{
			// memset(&client_addr, 0, sizeof(client_addr));
			client_len = sizeof(client_addr);
			// srv = 0;
		}
		~Client()
		{
			// if (fd > 0)
			// 	close(fd);
		}
		// void set_server(Server *s)
		// {
		// 	srv = s;
		// }
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
	private :
};

	#endif