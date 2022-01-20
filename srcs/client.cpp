/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 12:07:35 by edal--ce          #+#    #+#             */
/*   Updated: 2022/01/20 09:20:38 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(int new_listen_fd, list<Server*>& _servers) : _fd(0),_done_recv(0), _done_send(0), send_rdy(0), req(0), servers(_servers), _status(0)
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

int Client::status(void) const
{
	return _status;
}

void Client::set_response(void)
{
	send_rdy = 1;
	_done_send = 0;
	send_offset = 0;

	// if (req && _status != 4)
	// {
	// 	delete req;
	// 	req = 0;
	// }
	// if (_status != 4)
	// {
	// 	req = new Request(rec_buffer);
	// 	rec_buffer.clear();
	// }


	//Function that allows later calls to get the data
	req->prep_response(servers);

	if (req && req->get_type() == "POST" && req->get_s_header("Content-Type").find("multipart") != string::npos)
	{
		// DEBUG("We  are in the mode");
		// DEBUG("|" << req->get_s_header("Body")<< "|");
		if (req->get_s_header("Body").size() != static_cast<unsigned int>(atoi(req->get_s_header("Content-Length").c_str())))
		{
			// DEBUG("rcb is " << rec_buffer);
			// req->get_s_header("Body") += rec_buffer;
			// rec_buffer.clear();
			// Log("NEED TO GET MORE DATA");
			_status = 4;
			// DEBUG("We  are in the WTF");
			return;
		}
		else
			_status = 1;
	}

	unsigned int ctlen = atoi(req->get_s_header("Content-Length").c_str());
	// DEBUG("CTLEN IS " << ctlen);
	// DEBUG("CODE IS " << req->get_code());


	req->body_size_ok(ctlen);
	// todo:
	// if (req && ctlen > req->get_max_body() )
	// {
	// 	DEBUG("DATA IS TOO BIG");
	// 	r
	// 	req->code = 413;

	// 	// exit(0);
	// }
	// DEBUG("MAX BODY OK");
	// if (req->get_s_header("Content-Length"))

	req->set_filepath();
	int operation_status = req->get_file_status(_file_fd);
	DEBUG("OPSTAT IS " << operation_status);
	DEBUG("FILEPATH IS " << req->get_filepath());
	switch (operation_status)
	{
		case 0:
		{
			send_buffer = req->get_header(0, false);
			_status = 1;
			delete req;
			req = 0;
			break;
		}
		case 1:
		{
			string tmp;
			req->get_auto_index(tmp);
			send_buffer = req->get_header(tmp.size(), true);
			send_buffer += tmp;
			_status = 1;
			delete req;
			req = 0;
			break;
		}
		case 2:
		{
			// DEBUG("CODE B4 IS " << req->get_code());
			string tmp;
			req->launch_cgi(tmp, _file_fd);
			send_buffer = req->get_header(tmp.size(), true);
			send_buffer += tmp;
			_status = 1;
			_file_fd = 0;
			// DEBUG("CODE AF IS " << req->get_code());
			delete req;
			req = 0;
			break;
		}
		case 4:
		{
			req->delete_rq();
			_status = 1;
			send_buffer = req->get_header(0, 1);
			DEBUG("DELETE TIME STATUS IS 4");
			delete req;
			req = 0;
			break;
		}
		default:
			break;
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
	// DEBUG("received :" << string(buff, 0, len));
	if (len <= 0)
	{
		DEBUG("No data received, client is done");
		_done_recv = 1;
		return (-1);
	}
	if (req != NULL)
	{
		// DEBUG("BUFF|"<< string(buff, len)<<"|");
		req->give_head("Body") += string(buff, len); //todo: tu peux m'expliquer ca Dingzo pls ?
		// DEBUG("BODY IS " << req->get_s_header("Body"));
	}
	else
		rec_buffer += string(buff, len);

	if (len < BUFF_S)
	{
		// status = 1
		DEBUG("WE DONE RCV");
		if (req == NULL)
		{
			req = new Request(rec_buffer);
		}
		rec_buffer.clear();
		_done_recv = 1;
	}
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
		DEBUG("HEADER IS " << send_buffer);
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
	switch (_status)
	{
		case 1:
			this->send_header();
			break;
		case 2:
			Log("Sending file");
			this->send_fd();
			// Log("Done");
			break;
		default:
			break;
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

socklen_t* Client::get_addr_len(void)
{
	return (&client_len);
}

struct sockaddr* Client::get_sockaddr(void)
{
	return (struct sockaddr*)(&client_addr);
}