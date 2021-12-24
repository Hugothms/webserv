/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 12:07:35 by edal--ce          #+#    #+#             */
/*   Updated: 2021/12/24 16:26:09 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client() : _done_recv(0), _done_send(0), send_rdy(0), req(0)
{}

Client::Client(int new_listen_fd) : _done_recv(0), _done_send(0), send_rdy(0), req(0)
{
	_fd = accept(new_listen_fd, get_sockaddr(), get_addr_len());

	DEBUG("Client created with fd " << _fd);

	// inet_ntop(AF_INET, &(client_addr.sin_addr), client_ipv4_str, INET_ADDRSTRLEN);
	// printf("Incoming connection from %s:%d.\n", new_client->v4str(), new_client->client_addr.sin_port);
}

Client::~Client()
{
	if (_fd > 0)
		close(_fd);
	if (req != 0)
	{
		delete req;
		req = 0;
	}
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

bool Client::is_send_rdy() const
{
	return send_rdy;
}

void Client::set_response(void)
{
	send_rdy = 1;

	_done_send = 0;
	send_offset = 0;
	rec_buffer.clear();
	
	if (req->g_type().compare("POST") == 0 && req->get_s_header("Body").empty())
	{
		send_buffer = "HTTP/1.1 100 Continue";
	}
	else if (req->g_type() == "GET" || req->g_type() == "POST")
	{
		send_buffer = req->respond(servers);
	}
	else
	{
		DEBUG("OH NO UNSPPORTED TYPE");
	}
	// DEBUG("********************* RESPONSE *******************************");
	// DEBUG(send_buffer);
	// DEBUG("*********************** END **********************************");
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

int Client::receive(void)
{
	char buff[BUFF_S];
	int len;

	_done_recv = 0;

	len = recv(_fd, buff, BUFF_S, 0);
	if (len <= 0)
	{
		DEBUG("No data received, client is done");
		_done_recv = 1;
		return (-1);
	}

	if (req != 0) //If we are in post mode
		req->get_s_header("Body") += string(buff, len);
	else
		rec_buffer += string(buff, len);
	
	if (len < BUFF_S)
	{
		_done_recv = 1;
		if (req == 0)
			req = new Request(rec_buffer);
	}
	return (_done_recv);
}

void Client::send(void)
{
	int actual = BUFF_S;

	if (send_offset + actual > send_buffer.size())
		actual = send_buffer.size() - send_offset;

	::send(_fd, send_buffer.c_str() + send_offset, actual, 0);

	send_offset += actual;

	if (send_offset == send_buffer.size())
	{
		DEBUG("****** RESPONSE SENT *******");
		if (req != 0 && send_buffer.compare("HTTP/1.1 100 Continue") != 0)
		{
			delete req;
			req = 0;
		}
		_done_send = 1;
		send_rdy = 0;
		_done_recv = 0;
		send_buffer.clear();
	}
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
\
socklen_t* Client::get_addr_len(void)
{
	return (&client_len);
}

struct sockaddr* Client::get_sockaddr(void)
{
	return (struct sockaddr*)(&client_addr);
}