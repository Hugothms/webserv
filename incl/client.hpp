/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 23:06:00 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/26 13:30:00 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# ifndef CLIENT_HPP
# define CLIENT_HPP

# include "includes.hpp"
# include "server.hpp"
#include <sys/socket.h>


#define tmps 50


class Client
{
	private :
		
		int					_fd;
		bool 				_done_recv;
		bool 				_done_send;

	public :
		
		struct sockaddr_in	client_addr;
		char				client_ipv4_str[INET_ADDRSTRLEN];
		socklen_t			client_len;
		list<Server*>		servers;


		string				rec_buffer;
		char*				write_buffer;
		
		int 				blen;
			
		int 				write_pos;
		bool 				read_done;

		// string 				buffer;
		

		Client();
		~Client();
		Client(Server *srv);
		int receive();
		void set_fd(const int nfd);
		int get_fd(void) const;
		bool is_done_recv(void) const;
		void set_done_recv(bool t);
		void clear_recv(void);

		void push_back_server(Server *s);
		

		struct sockaddr* get_sockaddr(void);
		socklen_t *get_addr_len(void);
		char *v4str(void)
		{
			return client_ipv4_str;
		}
		// void identify(void)
		// {
		// 	cout << client_ipv4_str <<":" <<client_addr.sin_port << endl << endl;
		// }
};

#endif
