/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:58:06 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/18 12:28:58 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

void    CommandHandler::cmdMode( int client_socket, const std::string& param )
{
	std::istringstream  iss(param);
	std::string         channel_name, modes, arg, msg;
	Client*             target;
	Channel*            channel;

    iss >> channel_name;
	if (channel_name.empty())
		throw NeedMoreParamsException("MODE");

    channel = server->getChannel(channel_name);
    if (!channel)
		throw NoSuchChannel();

	if (!(iss >> modes))
	{
		std::string	modeString = channel->getModeString();
		std::string	params = channel->getModeParameters();
		std::string	reply = ":server 324 " + server->getClient(client_socket)->getNickname() + " " + channel_name + " " + modeString;

		if (!params.empty())
			reply += " " + params;
		reply += "\r\n";
		send(client_socket, reply.c_str(), reply.size(), 0);
		return ;
	}
	
    if (!channel->isOperator(server->getClient(client_socket)))
		throw ChanOPrivsNeeded(channel_name);

	bool	adding = true;
	size_t i = 0;
    while (i < modes.length())
    {
		char	ch = modes[i];

		if (ch == '+')
		{
			adding = true;
			i++;
			continue;
		}
		if (ch == '-')
		{
			adding = false;
			i++;
			continue;
		}

		switch (ch)
		{
			case 'i':
				channel->setInviteOnly(adding);
				break;
			case 't':
				channel->setTopicRestricted(adding);
				break;
			case 'k':
				if (adding)
				{
					if (!(iss >> arg))
						throw InvalidModeParam(channel_name, 'k', "");
					channel->setKey(arg);
				}
				else
					channel->setKey("");
				break;
			case 'l':
				if (adding)
            	{
					if (!(iss >> arg))
						throw InvalidModeParam(channel_name, 'l', "");
					std::istringstream	sarg(arg);
					int					limit;
					if (!(sarg >> limit))
						throw InvalidModeParam(channel_name, 'l', arg);
					channel->setUserLimit(limit);
            	}
            	else
                	channel->setUserLimit(-1);
				break;
			case 'o':
			{
				if (!(iss >> arg))
					throw InvalidModeParam(channel_name, 'o', "");
				target = server->getClientByNickname(arg);
				if (!target)
					throw NoSuchNick(arg);
				if (adding)
					channel->setOperator(target);
				else
					channel->removeOperator(target);
				break;
			}
			default:
				throw UModeUnkownFlag();
		}
		i++;
    }
    msg = ":" + (server->getClient(client_socket))->getNickname() + " MODE " + channel_name + " " + modes;
	std::istringstream	review(param);
	std::string			piece;
	
	review >> piece >> piece;
	while (review >> piece)
		msg += " " + piece;
	msg += "\r\n";

	broadcastToChannel(channel, msg);
}
