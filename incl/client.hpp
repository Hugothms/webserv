/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 23:06:00 by edal--ce          #+#    #+#             */
/*   Updated: 2021/12/07 17:40:46 by edal--ce         ###   ########.fr       */
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
		bool 				_done_recv;
		bool 				_done_send;



		string				rec_buffer;
		string 				data_buff;
		string				send_buffer;

	public :

		unsigned int 		send_offset;
		bool 				send_rdy;
		struct sockaddr_in	client_addr;
		char				client_ipv4_str[INET_ADDRSTRLEN];
		socklen_t			client_len;

		Request 			*req;


		list<Server*>		servers;




		int 				write_pos;
		bool 				read_done;



		Client();
		Client(int new_listen_fd);

		~Client();


		void set_fd(const int nfd);
		int get_fd(void) const;

		bool is_send_rdy() const;

		int receive(void);

		bool is_done_recv(void) const;
		void set_done_recv(bool t);
		void clear_recv(void);
		string *get_rec_buff(void);
		void set_response(void);

		void send(void);

		bool is_done_send(void) const;
		void set_done_send(bool t);
		void clear_send(void);
		string *get_send_buff(void);

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
