/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 23:06:00 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/12 23:07:00 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP


class Client
{
	public :
		int			fd;
		struct 		sockaddr_in client_addr;
		char		client_ipv4_str[INET_ADDRSTRLEN];
		socklen_t 	client_len;

		Client()
		{
			// memset(&client_addr, 0, sizeof(client_addr));
			client_len = sizeof(client_addr);
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
			cout << client_ipv4_str <<":" <<client_addr.sin_port << endl;
		}
	private :
};

	#endif