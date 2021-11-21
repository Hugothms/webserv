/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2021/11/21 18:57:12 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

Location::Location()
: max_client_body_size(1024), HTTP_redirection_type(0), autoindex(false)
{
	// map_pointer_function.insert(make_pair("HTTP_redirection", &(Location::set_HTTP_redirection)));
	// map_pointer_function.insert(make_pair("root", &(Location::set_path)));
	// map_pointer_function.insert(make_pair("default_answer", &(Location::set_default_answer)));
	// map_pointer_function.insert(make_pair("index", &(Location::set_index)));
	// map_pointer_function.insert(make_pair("upload_directory", &(Location::set_upload_directory)));
}

Location::~Location() {}

bool	Location::is_valid(string &error) const
{
	if (get_path() == "")
		error = (get_path() + " location: path is not set");
	list<string> allow = get_allow();
	if (allow.size() == 0)
		error = (get_path() + " location: allowed method(s) are not set");
	for (list<string>::iterator HTTP_method = allow.begin(); HTTP_method != allow.end(); HTTP_method++)
	{
		if (*HTTP_method == "POST" && get_upload_directory().length() == 0)
			error = (get_path() + " location: directory_upload is not set (and HTTP method POST is allowed)");
	}
	if (get_location_root() == "")
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

unsigned int	Location::get_max_client_body_size() const
{
	return (this->max_client_body_size);
}

unsigned int	Location::get_HTTP_redirection_type() const
{
	return (this->HTTP_redirection_type);
}

string			Location::get_HTTP_redirection() const
{
	return (this->HTTP_redirection);
}

string			Location::get_location_root() const
{
	return (this->location_root);
}

string			Location::get_index() const
{
	return (this->index);
}

string			Location::get_upload_directory() const
{
	return (this->upload_directory);
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

void Location::set_max_client_body_size(const unsigned int max_client_body_size)
{
	this->max_client_body_size = max_client_body_size;
}

void Location::set_HTTP_redirection_type(const unsigned int HTTP_redirection_type)
{
	this->HTTP_redirection_type = HTTP_redirection_type;
}

void Location::set_HTTP_redirection(const string HTTP_redirection)
{
	this->HTTP_redirection = HTTP_redirection;
}

void Location::set_location_root(const string location_root)
{
	this->location_root = location_root;
}

void Location::set_index(const string index)
{
	this->index = index;
}

void Location::set_upload_directory(const string upload_directory)
{
	this->upload_directory = upload_directory;
}

void Location::set_autoindex(const bool autoindex)
{
	this->autoindex = autoindex;
}
