/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2022/01/21 16:15:37 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

Location::Location()
: HTTP_redirection_type(0), autoindex(false)
{}

Location::~Location()
{
	return ;
}

bool	Location::is_valid(string &error) const
{
	if (get_path() == "")
		error = (get_path() + " location: path is not set");
	list<string> allow = get_allow();
	if (allow.size() == 0)
		error = (get_path() + " location: allowed method(s) are not set");
	if (get_root() == "")
		error = (get_path() + " location: location_root is not set");
	return (error.length() == 0);
}

Server			*Location::get_server() const
{
	return (this->server);
}

list<string>	Location::get_allow() const
{
	return (this->allow);
}

string			Location::get_path() const
{
	return (this->path);
}

unsigned int	Location::get_HTTP_redirection_type() const
{
	return (this->HTTP_redirection_type);
}

string			Location::get_HTTP_redirection() const
{
	return (this->HTTP_redirection);
}

string			Location::get_root() const
{
	return (this->location_root);
}

string			Location::get_index() const
{
	return (this->index);
}

bool			Location::get_autoindex() const
{
	return (this->autoindex);
}

void Location::set_server(Server *server)
{
	this->server = server;
}

void Location::set_allow(const list<string> allow)
{
	this->allow = allow;
}

void Location::push_back_HTTP_method(const string HTTP_method)
{
	this->allow.push_back(HTTP_method);
}
void Location::set_path(const string path)
{
	this->path = path;
}

void Location::set_HTTP_redirection_type(const unsigned int HTTP_redirection_type)
{
	this->HTTP_redirection_type = HTTP_redirection_type;
}

void Location::set_HTTP_redirection(const string HTTP_redirection)
{
	this->HTTP_redirection = HTTP_redirection;
}

void Location::set_root(const string location_root)
{
	this->location_root = location_root;
}

void Location::set_index(const string index)
{
	this->index = index;
}

void Location::set_autoindex(const bool autoindex)
{
	this->autoindex = autoindex;
}
