/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdPart.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:29:09 by rrichard          #+#    #+#             */
/*   Updated: 2025/04/26 09:34:07 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

void    CommandHandler::cmdPart( int client_socket, const std::string& param )
{
	std::string	channelsParam;
    Client*     client	= server->getClient(client_socket);

	std::istringstream	iss(param);
	if (!(iss >> channelsParam))
		throw NoSuchChannel();

	std::vector<std::string>	channelsList;
	size_t						start = 0, pos;
	while ((pos = channelsParam.find(',', start)) != std::string::npos)
	{
		channelsList.push_back(channelsParam.substr(start, pos - start));
		start = pos + 1;
	}
	if (start < channelsParam.size())
		channelsList.push_back(channelsParam.substr(start));

	std::string	reason;
	std::getline(iss >> std::ws, reason);
	for (size_t i = 0; i < channelsList.size(); i++)
	{
		std::string channel_name = channelsList[i];

		Channel*	channel = server->getChannel(channel_name);
		if (!channel)
			throw NoSuchChannel();
		if (!channel->hasClient(client))
			throw NotOnChannelException(channel_name);

		std::string msg;
		msg = getPrefix(client) + "PART " + channel->getName();
		if (!reason.empty())
			msg += " " + reason;
		msg += "\r\n";
		broadcastToChannel(channel, msg);
		channel->removeClient(client);
	}
}