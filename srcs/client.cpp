/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 12:07:35 by edal--ce          #+#    #+#             */
/*   Updated: 2021/11/08 12:23:39 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client()
{
	_fd = 0;
	_done_recv = 0;
	_done_send = 0;
	send_rdy = 0;
	client_len = sizeof(client_addr);
}

Client::Client(Server *srv)
{
	_done_recv = 0;
	_done_send = 0;
	send_rdy = 0;
	client_len = sizeof(client_addr);

	_fd = accept(srv->get_listen_fd(), get_sockaddr(), get_addr_len());

	DEBUG("Client created !");
	// inet_ntop(AF_INET, &(client_addr.sin_addr), client_ipv4_str, INET_ADDRSTRLEN);

	// printf("Incoming connection from %s:%d.\n", new_client->v4str(), new_client->client_addr.sin_port);
	
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

void Client::set_response(string str)
{
	send_buffer = str;
	send_rdy = 1;
	
	_done_send = 0;
	send_offset = 0;
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
	char buff[BUFF_S];
	int len;

	DEBUG("Receiving....");
	len = recv(_fd, buff, BUFF_S, 0);
	DEBUG("Received :" << len);

	if (len < BUFF_S && len > 0)
	{
		rec_buffer += string(buff, len);
		_done_recv = 1;
		DEBUG("Data fit in the buffer, read done");
		return 1;
	}
	else if (len == BUFF_S)
	{
		rec_buffer += string(buff, len);		
		_done_recv = 0;
		DEBUG("Data did not fit in the buffer, read more plz");
		return 0;
	}
	else
	{
		DEBUG("No data received, client is done");
		_done_recv = 1;
		return -1;
	}
	return (1);
}

int Client::send()
{
	int actual = BUFF_S;


	if (send_offset + actual > send_buffer.size())
		actual = send_buffer.size() - send_offset;
	if (actual <= 0)
	{
		// DEBUG("RET");
		return 0;
	}
	DEBUG("actual = " << actual);
	DEBUG("Sending response");
	::send(_fd, send_buffer.c_str() + send_offset, actual, 0);
	DEBUG("Sent from byte :" << send_offset << " to :" << send_offset + actual);
	send_offset += actual;

	if (send_offset == send_buffer.size())
	{
		_done_send = 1;
		send_rdy = 0;
		send_buffer.clear();
		_done_recv = 0;
		DEBUG("Send_rdy back to 0");
		
	}
	return 0;
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