/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdInvite.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:51:42 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/18 16:47:53 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

void    CommandHandler::cmdInvite(int client_socket, const std::string& param)
{
    std::istringstream  iss(param);
    std::string         target_nick, channel_name;
    Client*             inviter;
    Client*             target;
    Channel*            channel;
    std::string         msg;

    iss >> target_nick >> channel_name;
    if (target_nick.empty() || channel_name.empty())
        throw NeedMoreParamsException("INVITE");
    
    inviter = server->getClient(client_socket);
    target  = server->getClientByNickname(target_nick);
    if (!target)
        throw NoSuchNick(target_nick);
    
    channel = server->getChannel(channel_name);
    if (!channel)
        throw NoSuchChannel();
    if (!channel->hasClient(inviter) || !channel->isOperator(inviter))
        throw ChanOPrivsNeeded(channel_name);

    channel->addInvite(target);
    
    msg = ":" + inviter->getNickname() + " INVITE " + target_nick + " :" + channel_name + "\r\n";
    send(target->getSocket(), msg.c_str(), msg.size(), 0);
}
