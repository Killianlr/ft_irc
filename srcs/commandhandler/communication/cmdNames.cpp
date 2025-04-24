/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdNames.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:26:51 by rrichard          #+#    #+#             */
/*   Updated: 2025/04/24 18:12:49 by rrichard         ###   ########.fr       */
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
	std::string reply = ":ft_irc 353 " + client->getNickname() + " = " + channel->getName() + " :" + names + "\r\n";
	send(client->getSocket(), reply.c_str(), reply.size(), 0);

	// RPL_ENDOFNAMES (366)
	reply = ":ft_irc 366 " + client->getNickname() + " " + channel->getName() + " :End of /NAMES list.\r\n";
	send(client->getSocket(), reply.c_str(), reply.size(), 0);
}

void	CommandHandler::cmdNames( int client_socket, const std::string& param )
{
	Client* 					client = server->getClient(client_socket);
	std::vector<std::string>	channelsList;

	if (param.empty())
	{
		const std::map<std::string, Channel*>& chans = server->getChannels();
		for (std::map<std::string, Channel*>::const_iterator it = chans.begin(); it != chans.end(); ++it)
			channelsList.push_back(it->first);
	}
	else
	{
		std::string channelsParam = param;
		size_t start = 0, pos;
		while ((pos = channelsParam.find(',', start)) != std::string::npos)
		{
			channelsList.push_back(channelsParam.substr(start, pos - start));
			start = pos + 1;
		}
		if (start < channelsParam.size())
			channelsList.push_back(channelsParam.substr(start));
	}

	for (size_t i = 0; i < channelsList.size(); ++i)
	{
		const std::string&	chanName = channelsList[i];
		Channel*			channel = server->getChannel(chanName);

		if (channel)
			sendNamesList(channel, client);
		else
		{
			std::string reply = ":ft_irc 366 " + client->getNickname() + " " + chanName + " :End of /NAMES list.\r\n";
			send(client->getSocket(), reply.c_str(), reply.size(), 0);
		}
	}
}