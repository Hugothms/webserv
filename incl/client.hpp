/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 23:06:00 by edal--ce          #+#    #+#             */
/*   Updated: 2022/01/07 08:43:55 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# ifndef CLIENT_HPP
# define CLIENT_HPP

# include "includes.hpp"
# include "server.hpp"
#include <sys/socket.h>


#define BUFF_S 4096


class Client
{
	private :

		int					_fd;
		int 				_file_fd;
		bool 				_done_recv;
		bool 				_done_send;
		unsigned int 		send_offset;
		bool 				send_rdy;
		string				rec_buffer;
		string				send_buffer;
		Request 			*req;
		list<Server*>		servers;
		int 				_status;

		struct sockaddr_in	client_addr;
		char				client_ipv4_str[INET_ADDRSTRLEN];
		socklen_t			client_len;

		Client();

	public :

		Client(int new_listen_fd);

		~Client();

		void set_fd(const int nfd);
		int get_fd(void) const;


		int receive(void);
		int  status(void) const;
		bool is_done_recv(void) const;
		void set_done_recv(bool t);

		void set_response(void);

		void send(void);

		bool is_done_send(void) const;

		void push_back_server(Server *s);

		void send_header(void);
		void smart_send(void);
		void send_fd(void);

		struct sockaddr* get_sockaddr(void);
		socklen_t *get_addr_len(void);
};

#endif
