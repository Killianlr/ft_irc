/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdJoin.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:57:48 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/16 15:04:51 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

void    CommandHandler::cmdJoin( int client_socket, const std::string& param )
{
	std::string	response;
	std::string	channel_name = param;
	Client*		client = server->getClient(client_socket);
	Channel*	channel = server->getChannel(channel_name);

	std::cout << "param : " << param << std::endl;
	if (param.empty() || param[0] != '#')
		throw NoSuchChannel();
	if (channel)
	{
		std::cout << "Channel : " << channel_name << " already exist" << std::endl;
		// channel = server->getChannel(channel_name);
		// if (channel->getNbMembers() > )
		if (channel->isInviteOnly() && !channel->isInvite(client))
		{
			std::cout << "You're not invited to this channel\n" << std::endl; // MUST BE A EXCEPTION
			return ;
		}
	}
	else
	{
		std::cout << "Channel : " << channel_name << " be created" << std::endl;
		channel = new Channel(channel_name);
		server->addChannel(channel_name, channel);
		client->setOperators();
	}
	channel->addClient(client);

	response = ":" + client->getNickname() + " JOIN " + channel_name + "\r\n";
	send(client_socket, response.c_str(), response.size(), 0);
}