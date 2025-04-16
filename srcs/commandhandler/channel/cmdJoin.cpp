/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdJoin.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:57:48 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/16 18:01:27 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

void    CommandHandler::cmdJoin( int client_socket, const std::string& param )
{
	std::string	channelsParam, keysParam, response;
	Client*		client = server->getClient(client_socket);

	std::istringstream	iss(param);
	if (!(iss >> channelsParam))
		throw NoSuchChannel();
	iss >> keysParam;

	std::vector<std::string>	channelsList;
	size_t						start = 0, pos;
	while ((pos = channelsParam.find(',', start)) != std::string::npos)
	{
		channelsList.push_back(channelsParam.substr(start, pos - start));
		start = pos + 1;
	}
	if (start < channelsParam.size())
		channelsList.push_back(channelsParam.substr(start));

	std::vector<std::string>	keysList;
	if (!keysParam.empty())
	{
		start = 0;
		while ((pos = keysParam.find(',', start) != std::string::npos))
		{
			keysList.push_back(keysParam.substr(start, pos - start));
			start = pos + 1;
		}
		if (start < keysParam.size())
			keysList.push_back(keysParam.substr(start));
	}

	for (size_t i = 0; i < channelsList.size(); i++)
	{
		std::string channel_name = channelsList[i];
		if (channel_name.empty() || channel_name[0] != '#')
			throw NoSuchChannel();
		Channel*	channel = server->getChannel(channel_name);
		std::string	key = "";
		if (i < keysList.size())
			key = keysList[i];
		
		if (!channel)
		{
			channel = new Channel(channel_name);
			server->addChannel(channel_name, channel);
			channel->setOperator(client);
		}
		else if (channel->isInviteOnly() && !channel->isInvite(client))
			throw InviteOnlyChan(channel->getName());

		channel->addClient(client);
		response = ":" + client->getNickname() + " JOIN " + channel_name + "\r\n";
		send(client_socket, response.c_str(), response.size(), 0);
		std::string topicMsg = ":server 332 " + client->getNickname() + " " + channel_name + " :" + channel->getTopic() + "\r\n";
		send(client_socket, topicMsg.c_str(), topicMsg.size(), 0);
	}
}