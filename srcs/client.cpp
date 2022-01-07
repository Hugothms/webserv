/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 12:07:35 by edal--ce          #+#    #+#             */
/*   Updated: 2022/01/07 08:41:51 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client() :_fd(0), _done_recv(0), _done_send(0), send_rdy(0), req(0), _status(0)
{
	client_len = sizeof(client_addr);
}

Client::Client(int new_listen_fd) : _fd(0),_done_recv(0), _done_send(0), send_rdy(0), req(0), _status(0)
{
	client_len = sizeof(client_addr);
	_fd = accept(new_listen_fd, get_sockaddr(), get_addr_len());

	inet_ntop(AF_INET, &(client_addr.sin_addr), client_ipv4_str, INET_ADDRSTRLEN);
	Log("New client from " + string(client_ipv4_str), GREEN);
}

Client::~Client()
{
	DEBUG("Killing _fd = " << _fd);
	Log("Client disconnected");
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
/*string* Client::get_rec_buff(void)
{
	return (&rec_buffer);
}*/

// string* Client::get_send_buff(void)
// {
// 	return (&send_buffer);
// }

// bool Client::is_send_rdy() const
// {
// 	return send_rdy;
// }

int Client::status(void) const
{
	return _status;
}

void Client::set_response(void)
{
	send_rdy = 1;
	_done_send = 0;
	send_offset = 0;


	if (req && _status != 4)
		delete req;
	if (_status != 4)
		req = new Request(rec_buffer);

	// if ()

	if (req && req->g_type() == "POST" && req->get_s_header("Content-Type").find("multipart") != string::npos)
	{
		// Log("We  are in the mode", GREEN);
		if (req->get_s_header("Body").size() != static_cast<unsigned int>(atoi(req->get_s_header("Content-Length").c_str())))
		{
			_status = 4;
			return;
		}
		else
			_status = 1;
	}

	rec_buffer.clear();

	//Function that allows later calls to get the data
	req->prep_response(servers);
	req->set_filepath();

	int operation_status = req->get_file_status(_file_fd);

	if (operation_status == 0)
	{
		send_buffer = req->get_header(0, false);
		_status = 1;
		return ;
	}
	else if (operation_status == 1)
	{
		string tmp;
		req->get_auto_index(tmp);

		send_buffer = req->get_header(tmp.size(), true);
		send_buffer += tmp;

		_status = 1;

	}
	else if (operation_status == 2)
	{
		string tmp;
		req->launch_cgi(tmp, _file_fd);

		send_buffer = req->get_header(tmp.size(), true);
		send_buffer += tmp;
		_status = 1;
		_file_fd = 0;
	}
}

bool Client::is_done_send(void) const
{
	return _done_send;
}


int Client::receive(void)
{
	char buff[BUFF_S];

	_done_recv = 0;

	int len = recv(_fd, buff, BUFF_S, 0);
	DEBUG("received");
	if (len <= 0)
	{
		DEBUG("No data received, client is done");
		_done_recv = 1;
		return (-1);
	}

	if (req != 0)
		req->get_s_header("Body") += string(buff, len);
	else
		rec_buffer += string(buff, len);

	if (len < BUFF_S)
		_done_recv = 1;

	return (_done_recv);
}

void Client::send_header(void)
{
	int actual = BUFF_S;

	if (send_offset + actual > send_buffer.size())
		actual = send_buffer.size() - send_offset;

	::send(_fd, send_buffer.c_str() + send_offset, actual, 0);

	send_offset += actual;

	if (send_offset == send_buffer.size())
	{
		send_buffer.clear();
		_status = 2;
	}
}

void Client::send_fd(void)
{
	char 	buff[BUFF_S];
	int 	s_read;

	if (_file_fd == 0)
	{
		delete req;
		_done_recv = _status = 0;
		req = NULL;
		return ;
	}

	s_read = read(_file_fd, buff, BUFF_S);

	if (s_read <= 0)
	{
		close(_file_fd);
		delete req;
		_file_fd = _done_recv = _status = 0;
		req = NULL;
		return;
	}
	::send(_fd, buff, s_read, 0);
	if (s_read < BUFF_S)
	{
		Log("Done sending file to client", YELLOW);
		close(_file_fd);
		delete req;
		_status = _done_recv = _file_fd = 0;
		req = NULL;
	}
}

void Client::smart_send(void)
{
	if (_status == 1)
		this->send_header();
	else if (_status == 2)
		this->send_fd();
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