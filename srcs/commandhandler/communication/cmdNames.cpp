/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdNames.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:26:51 by rrichard          #+#    #+#             */
/*   Updated: 2025/04/18 14:58:15 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

void	CommandHandler::sendNamesList( Channel* channel, Client* client )
{
	// RPL_NAMREPLY (353)
	std::string	names;
	const std::vector<Client*>&	users = channel->getMembers();
	for (size_t i = 0; i < users.size(); i++)
	{
		if (channel->isOperator(users[i]))
			names += "@";
		names += users[i]->getNickname();
		if (i + 1 < users.size())
			names += " ";
	}
	std::string reply = ":server 353 " + client->getNickname() + " = " + channel->getName() + " :" + names + "\r\n";
	send(client->getSocket(), reply.c_str(), reply.size(), 0);

	// RPL_ENDOFNAMES (366)
	reply = ":server 366 " + client->getNickname() + " " + channel->getName() + " :End of /NAMES list.\r\n";
	send(client->getSocket(), reply.c_str(), reply.size(), 0);
}

void	CommandHandler::cmdNames( int client_socket, const std::string& param )
{
	std::string					channelsParam;
	std::vector<std::string>	channelsList;
	size_t						start = 0, pos;

	while ((pos = channelsParam.find(',', start)) != std::string::npos)
	{
		channelsList.push_back(channelsParam.substr(start, pos - start));
		start = pos + 1;
	}
	if (start < channelsParam.size())
		channelsList.push_back(channelsParam.substr(start));

	if (param.empty())
	{
		// std::string reply = ":server 366 " + client->getNickname() + " " + channel->getName() + " :End of /NAMES list.\r\n";
		// send(client->getSocket(), reply.c_str(), reply.size(), 0);
		return ;
	}
	else
	{
		Channel*	channel = server->getChannel(param);
		if (channel)
			sendNamesList(channel, server->getClient(client_socket));
	}
}