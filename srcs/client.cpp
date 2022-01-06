/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 12:07:35 by edal--ce          #+#    #+#             */
/*   Updated: 2022/01/06 10:53:15 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client() :_fd(0), _done_recv(0), _done_send(0), send_rdy(0), req(0), status(0)
{
	client_len = sizeof(client_addr);
}

Client::Client(int new_listen_fd) : _fd(0),_done_recv(0), _done_send(0), send_rdy(0), req(0), status(0)
{
	client_len = sizeof(client_addr);
	_fd = accept(new_listen_fd, get_sockaddr(), get_addr_len());

	Log("New client", GREEN);
	
	// DEBUG("Client created with fd " << _fd);

	// inet_ntop(AF_INET, &(client_addr.sin_addr), client_ipv4_str, INET_ADDRSTRLEN);
	// printf("Incoming connection from %s:%d.\n", new_client->v4str(), new_client->client_addr.sin_port);
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

	if (req)
		delete req;
	req = new Request(rec_buffer);
	
	rec_buffer.clear();
	

	//Function that allows later calls to get the data
	req->prep_response(servers);
	Log("prep response OK");
	req->set_filepath();
	Log("set filepath OK");


	if (req->g_type() == "GET" && req->get_s_header("Content-Type").find("multipart") != string::npos)
	{
		Log("We  are in the mode", GREEN);
	}
	
	// int nfd = 0;
	int operation_status = req->get_file_status(_file_fd);
	Log("File status OK");

	if (operation_status == 0)
	{
		// Log("OPSTAT = 0, WE HAVE THE DATA TO SEND");
		send_buffer = req->get_normal_header();
		// Log("Prepeed resp is : \n" + send_buffer);

		status = 1;
		return ;
	}
	else if (operation_status == 1)
	{
		string tmp;
		req->get_auto_index(tmp);
		
		send_buffer = req->get_index_header(tmp);
		send_buffer += tmp;

		status = 1;
		
	}
	else if (operation_status == 2)
	{
		string tmp;
		req->launch_cgi(tmp, _file_fd);
		
		send_buffer = req->get_cgi_header(tmp.size());
		send_buffer += tmp;
		status = 1;
		
		_file_fd = 0;
		//This is a CGI
	}

	return ;


	

	// if (req && (req->g_type().compare("POST") == 0 && req->get_s_header("Body").empty()))
	// {
	// 	send_buffer = "HTTP/1.1 100 Continue";
	// }
	// else if (req && (req->g_type() == "GET" || (req->g_type() == "POST" || fast_pipe > 0)))
	// {
	// 	send_buffer = req->respond(servers);
	// }
	// else
	// {
	// 	DEBUG("OH NO UNSPPORTED TYPE");
	// }
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

	// if (req != 0 && fast_pipe) //If we are in post mode
	// {
	// 	//We should fill a buffer to sendin
	// 	rec_buffer.clear();
	// 	rec_buffer += string(buff, len);
	// }
	
	if (req != 0)
	{
		req->get_s_header("Body") += string(buff, len);
		DEBUG("Storing in body");
	} //If we are in post mode, putting in the header field
		
	else
	{
		rec_buffer += string(buff, len);
		// DEBUG("Rbuff:");
	}
	
	

	if (len < BUFF_S)
	{
		DEBUG("rec_buff : " << rec_buffer);
		DEBUG("_done_recv = 1");
		_done_recv = 1;
	}


	return (_done_recv);
}

void Client::send_header(void)
{
	// static string test;

	int actual = BUFF_S;

	if (send_offset + actual > send_buffer.size())
		actual = send_buffer.size() - send_offset;


	::send(_fd, send_buffer.c_str() + send_offset, actual, 0);

	send_offset += actual;

	if (send_offset == send_buffer.size())
	{
		// µ%£M12345+°./§§%µ£
		DEBUG("SEND HEADER DONE:|" << send_buffer << "|");

		// DEBUG("send_buffer");
		


		// if (req != 0 && send_buffer.compare("HTTP/1.1 100 Continue") != 0)
		// {
		// 	delete req;
		// 	req = 0;
		// }
		// _done_send = 1;
		// send_rdy = 0;
		// _done_recv = 0;
		send_buffer.clear();
		status = 2;
	}
}

void Client::send_fd(void)
{
	static string tmp;

	if (_file_fd == 0)
	{
		tmp.clear();
		_done_recv = 0;
		delete req;
		req = 0;
		status = 0;
		return ;
	}

	char buff[BUFF_S];

	// Log("Send FD");
	int s_read = read(_file_fd, buff, BUFF_S);
	

	tmp += string(buff, 0, BUFF_S);
	// DEBUG("Read is " << s_read);
	if (s_read <= 0)
	{
		Log("s_read 0");
		_done_recv = 0;
		close(_file_fd);

		_file_fd = 0;
		status = 0;
		delete req;
		req = 0;
		return;
	}
	Log("Sending_fd..");
	::send(_fd, buff, s_read, 0);
	Log("Done");
	if (s_read < BUFF_S)
	{
		Log("send_fd complete");
		// DEBUG("FD_SEND DONE :|" << tmp << "|");
		tmp.clear();
		close(_file_fd);
		_done_recv = 0;
		_file_fd = 0;
		status = 0;
		delete req;
		req = 0;
		//All sent
	}



}

void Client::smart_send(void)
{
	//1 Means we have the header ready and the FD in store
	if (status == 1)
	{
		this->send_header();
	}
	else if (status == 2) //Send from the FD
	{
		this->send_fd();
		// _done_recv = 0;
		// Log("Now send the rest");
	}	
}

void Client::send(void)
{
	int actual = BUFF_S;

	if (send_offset + actual > send_buffer.size())
		actual = send_buffer.size() - send_offset;

	// if (fast_pipe == 2)
	// {
	// 	//We send 100 Continue
	// 	::send(_fd, send_buffer.c_str(), send_buffer.size(), 0);
		



	// 	fast_pipe = 1;
	// 	return;
	// }

	// if (fast_pipe == 1)
	// {
	// 	char buff[BUFF_S];

	


	// 	::send(_fd, rec_buffer.c_str(), rec_buffer.size(), 0);
		
	// 	if (file_done)

	// 	return;
	// }

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