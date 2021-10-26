/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 12:07:35 by edal--ce          #+#    #+#             */
/*   Updated: 2021/10/26 14:19:16 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client()
{
	_fd = 0;
	_done_recv = 0;
	_done_send = 0;
	client_len = sizeof(client_addr);
}

Client::Client(Server *srv)
{
	_done_recv = 0;
	_done_send = 0;
	client_len = sizeof(client_addr);

	
	DEBUG("New conn incomming, need to accept it !");

	_fd = accept(srv->get_listen_fd(), get_sockaddr(), get_addr_len());

	// inet_ntop(AF_INET, &(client_addr.sin_addr), client_ipv4_str, INET_ADDRSTRLEN);

	// printf("Incoming connection from %s:%d.\n", new_client->v4str(), new_client->client_addr.sin_port);
	DEBUG("Client created !");
}


Client::~Client()
{
	if (_fd > 0)
		close(_fd);
	
	DEBUG("CLIENT KILLED\n");
}

bool Client::is_done_recv(void) const
{
	return _done_recv;
}
void Client::set_done_recv(bool t)
{
	_done_recv = t;
}
string* Client::get_rec_buff(void)
{
	return (&rec_buffer);
}

string* Client::get_send_buff(void)
{
	return (&send_buffer);
}

void Client::clear_recv(void)
{
	rec_buffer.clear();
}

bool Client::is_done_send(void) const
{
	return _done_send;
}
void Client::set_done_send(bool t)
{
	_done_send = t;
}

void Client::clear_send(void)
{
	send_buffer.clear();
}

int Client::receive()
{
	char buff[tmps];			
	int len;

		len = recv(_fd, buff, tmps, 0);
		if (len < tmps && len > 0)
		{
			rec_buffer += string(buff, len);
			_done_recv = 1;
			DEBUG("FINISHED READING\n");
			return 1;
		}
		else if (len == tmps)
		{
			rec_buffer += string(buff, len);
			_done_recv = 0;
			return 0;
		}
		else
		{
			_done_recv = 1;
			DEBUG("FINISHED READING OR DC\n");	
			return -1;
		}
	return (1);
}


void Client::set_fd(const int fd)
{
	_fd = fd;	
}
int Client::get_fd(void) const
{
	return _fd;
}

void Client::push_back_server(Server *s)
{
	servers.push_back(s);
}


socklen_t* Client::get_addr_len(void)
{
	return (&client_len);
}

struct sockaddr* Client::get_sockaddr(void)
{
	return (struct sockaddr*)(&client_addr);
}