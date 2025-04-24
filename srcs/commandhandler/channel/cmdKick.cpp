/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdKick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:58:18 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/24 17:20:50 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

void    CommandHandler::cmdKick(int client_socket, const std::string& param)
{
	std::istringstream			iss(param);
	std::string					channel_name, targets, reason;
	Channel*					channel;
	Client*						kicker;
	Client*						target;
	std::string					msg;

	iss >> channel_name >> targets;

	if (channel_name.empty() || targets.empty())
		throw NeedMoreParamsException("KICK");

	std::vector<std::string>	nicksList;
	size_t						start = 0, pos;
	while ((pos = targets.find(',', start)) != std::string::npos)
	{
		nicksList.push_back(targets.substr(start, pos - start));
		start = pos + 1;
	}
	if (start < targets.size())
		nicksList.push_back(targets.substr(start));

	std::getline(iss >> std::ws, reason);
	if (reason.empty())
		reason = "no particular reason";

	channel = server->getChannel(channel_name);
	if (!channel)
		throw NoSuchChannel();
	
	kicker = server->getClient(client_socket);
	if (!channel->isOperator(kicker))
		throw ChanOPrivsNeeded(channel_name);
	
	for (size_t i = 0; i < nicksList.size(); i++)
	{
		std::string	target_nick = nicksList[i];
		target = server->getClientByNickname(target_nick);
		if (!target || !channel->hasClient(target))
			throw UserNotInChannel(target_nick, channel_name);
		msg = getPrefix(kicker) + "KICK " + channel_name + " " + target_nick + " " + reason + "\r\n";
		broadcastToChannel(channel, msg);
		channel->removeClient(target);
	}
}
