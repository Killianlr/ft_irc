/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdTopic.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:58:26 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/16 16:48:31 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

void	CommandHandler::cmdTopic( int socket, const std::string& param )
{
	std::istringstream	iss(param);
	std::string			target, topic;
	Channel*			current;
	Client*				requester = server->getClient(socket);

	if (!(iss >> target))
		throw NeedMoreParamsException("TOPIC");
	
	std::getline(iss >> std::ws, topic);
	current = server->getChannel(target);
	if (!current)
		throw NoSuchChannel();

	bool isMember = false;
	const std::vector<Client*>&	members = current->getMembers();
	for (std::vector<Client*>::const_iterator it = members.begin(); it != members.end(); it++)
	{
		if ((*it)->getNickname() == requester->getNickname())
		{
			isMember = true;
			break ;
		}
	}
	if (!isMember)
		throw NotOnChannelException(target);

	if (topic.empty())
	{
		std::string	response;
		if (current->getTopic().empty())
			response = ":server 331 " + requester->getNickname() + " " + target + " :No topic is set\r\n";
		else
			response = ":server 332 " + requester->getNickname() + " " + target + " :" + current->getTopic() + "\r\n";
		send(socket, response.c_str(), response.size(), 0);
		return ;
	}

	if (current->isTopicRestricted() && !current->isOperator(server->getClient(socket)))
		throw ChanOPrivsNeeded(target);
	current->setTopic(topic);
	std::string	msg = ":" + requester->getNickname() + " TOPIC " + target + " :" + topic + "\r\n";
	broadcastToChannel(current, msg);
}
