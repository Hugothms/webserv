/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2021/11/18 17:56:32 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

Location::Location()
: autoindex(false)
{
	// map_pointer_function.insert(make_pair("HTTP_redirection", &(Location::set_HTTP_redirection)));
	// map_pointer_function.insert(make_pair("root", &(Location::set_path)));
	// map_pointer_function.insert(make_pair("default_answer", &(Location::set_default_answer)));
	// map_pointer_function.insert(make_pair("index", &(Location::set_index)));
	// map_pointer_function.insert(make_pair("upload_directory", &(Location::set_upload_directory)));
}

Location::~Location() {}

string		Location::is_valid(void) const
{
	if (get_path() == "")
		return (get_path() + " location: path is not set");
	list<string> allow = get_allow();
	if (allow.size() == 0)
		return (get_path() + " location: allow is not set");
	for (list<string>::iterator HTTP_method = allow.begin(); HTTP_method != allow.end(); HTTP_method++)
	{
		if (*HTTP_method == "POST" && get_upload_directory().length() == 0)
			return "directory_upload is not set (and HTTP_method POST is accepted)";
	}
	if (get_location_root() == "")
		return (get_path() + " location: location_root is not set");
	return "";
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
