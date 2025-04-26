/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:00:09 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/25 23:13:03 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( int socket ) : socket(socket), nickname(""), username(""), realname(""), authenticated(false), registered(false), operators(false) {}

Client::Client() : socket(0), nickname(""), username(""), realname(""), authenticated(false), registered(false), operators(false) {}

Client::Client( const Client& other ) : socket(other.socket), nickname(other.nickname), username(other.username), realname(other.realname), authenticated(other.authenticated), registered(other.registered), operators(other.operators) {}

Client&	Client::operator=( const Client& other )
{
	if (this != &other)
	{
		this->socket = other.socket;
		this->nickname = other.nickname;
		this->username = other.username;
		this->realname = other.realname;
		this->authenticated = other.authenticated;
		this->registered = other.registered;
		this->operators = other.operators;
		this->channels = other.channels;
	}
	return (*this);
}

Client::~Client() {}

int Client::getSocket()
{
	return (this->socket);
}

const std::string&  Client::getNickname() const
{
	return (this->nickname);
}

const std::string&  Client::getUsername() const
{
	return (this->username);
}

const std::string&  Client::getRealname() const
{
	return (this->realname);
}

bool	Client::isAuthenticated()
{
	return (this->authenticated);
}

bool	Client::isRegistered()
{
	return (this->registered);
}

bool	Client::isOperators()
{
	return (this->operators);
}

void	Client::setNickname( const std::string& str )
{
	this->nickname = str;
}

void	Client::setUsername( const std::string& str )
{
	this->username = str;
}

void	Client::setRealname( const std::string& str )
{
	this->realname = str;
}

void	Client::setAuthenticated()
{
	if (this->authenticated != true)
		this->authenticated = true;
}

void	Client::setRegistered()
{
	if (this->registered != true)
		this->registered = true;
}

void	Client::setOperators()
{
	if (this->operators != true)
		this->operators = true;
}

const std::vector<Channel*>&	Client::getChannels() const
{
	return (this->channels);
}

void	Client::addChannel( Channel* channel )
{
	this->channels.push_back(channel);
}

void	Client::removeChannel( Channel* channel )
{
	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (channel == (*it))
		{
			it = channels.erase(it);
			break;
		}
	}
}

void	Client::removeChannel( const std::string& channel )
{
	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (channel == (*it)->getName())
		{
			it = channels.erase(it);
			break;
		}
	}
}