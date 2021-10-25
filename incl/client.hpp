/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 23:06:00 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/25 23:14:24 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "includes.hpp"
# include "server.hpp"
#include <sys/socket.h>


#define tmps 50


class Client
{
	private :
	public :
		int					fd;
		struct sockaddr_in	client_addr;
		char				client_ipv4_str[INET_ADDRSTRLEN];
		socklen_t			client_len;
		list<Server*>		servers;


		char*				rec_buffer;
		char*				write_buffer;
		
		int 				blen;
			
		int 				write_pos;
		bool 				read_done;

		// string 				buffer;
		

		Client()
		{
			// memset(&client_addr, 0, sizeof(client_addr));
			fd = 0;
			rec_buffer = 0;
			client_len = sizeof(client_addr);
		}
		void receive()
		{
			// int len = 0;

			if (rec_buffer == 0)
			{
				
				rec_buffer = new char[tmps];
				DEBUG("BUFFER ALLOCATED\n");
			}
			
			int test =1;

			
			while((test = recv(fd, rec_buffer, tmps, 0)) > 0)
			{
				if (test < tmps)
				{
					
					DEBUG("WE DID RECEIVE :" << test);
					write(2, rec_buffer, test);
				}
				else
				{
					write(2, rec_buffer, test);
					DEBUG("STILL MORE TO RECV\n");
				}	
			}

			
			


		}
		void set_fd(const int nfd)
		{
			fd = nfd;
		}
		char *get_rec_buff()
		{
			return rec_buffer;	
		}
		int get_fd(void)
		{
			return fd;
		}
		~Client()
		{
			DEBUG("KILLING CLIENT\n");
			delete rec_buffer;
			if (fd > 0)
			{
				close(fd);
				DEBUG("KILLED\n");
			}
		}
		void push_back_server(Server *s)
		{
			servers.push_back(s);
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
