/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:04:40 by edal--ce          #+#    #+#             */
/*   Updated: 2021/11/02 13:57:20 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

Location::Location()
: directory_listing(false)
{}

Location::~Location() {}

list<string>	Location::get_HTTP_methods() const
{
	return (this->HTTP_methods);
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

string			Location::get_default_answer() const
{
	return (this->default_answer);
}

string			Location::get_index() const
{
	return (this->index);
}

string			Location::get_upload_directory() const
{
	return (this->upload_directory);
}

bool			Location::get_directory_listing() const
{
	return (this->directory_listing);
}

void Location::set_HTTP_methods(const list<string> HTTP_methods)
{
	this->HTTP_methods = HTTP_methods;
}

void Location::push_back_HTTP_method(const string HTTP_method)
{
	this->HTTP_methods.push_back(HTTP_method);
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

void Location::set_default_answer(const string default_answer)
{
	this->default_answer = default_answer;
}

void Location::set_index(const string index)
{
	this->index = index;
}

void Location::set_upload_directory(const string upload_directory)
{
	this->upload_directory = upload_directory;
}

void Location::set_directory_listing(const bool directory_listing)
{
	this->directory_listing = directory_listing;
}
