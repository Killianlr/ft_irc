/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:58:06 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/17 18:39:14 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

void    CommandHandler::cmdMode(int client_socket, const std::string &param)
{
    std::istringstream  iss(param);
    std::string         channel_name, modes, params, msg;
    Client*             target;
    Channel*            channel;
    bool                adding = false;

    iss >> channel_name >> modes >> params;
    
    channel = server->getChannel(channel_name);
    if (!channel)
		throw NoSuchChannel();

    if (!channel->isOperator(server->getClient(client_socket)))
		throw ChanOPrivsNeeded(channel_name);

	if (modes[0] == '+')
		adding = true;
    for (size_t i = 1; i < modes.length(); ++i)
    {
		switch (modes[i])
		{
			case 'i':
				channel->setInviteOnly(adding);
				break;
			case 't':
				channel->setTopicRestricted(adding);
				break;
			case 'k':
			{
				if (adding)
				{
					if (params.empty())
						throw InvalidModeParam(channel_name, modes[i], params);
					channel->setKey(params);
				}
				else
					channel->setKey("");
				break;
			}
			case 'l':
			{
				if (adding)
            	{
					std::istringstream	isslimit(params);
					int					limit;

					if (!(iss >> limit))
						throw InvalidModeParam(channel_name, 'l', params);
					channel->setUserLimit(limit);
            	}
            	else
                	channel->setUserLimit(-1);
				break;
			}
			case 'o':
			{
				if (params.empty())
					throw InvalidModeParam(channel_name, modes[i], params);
				target = server->getClientByNickname(params);
				if (!target)
					throw NoSuchNick(params);
				if (adding)
					channel->setOperator(target);
				else
					channel->setOperator(target);
				break;
			}
			default:
				throw UModeUnkownFlag();
		}
    }
    msg = ":" + (server->getClient(client_socket))->getNickname() + " MODE #" + channel_name + " " + modes + "\r\n";
	broadcastToChannel(channel, msg);
}
