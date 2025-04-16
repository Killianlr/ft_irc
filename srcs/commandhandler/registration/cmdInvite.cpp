/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdInvite.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:51:42 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/16 14:59:02 by rrichard42       ###   ########.fr       */
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
        throw IRCException("Error: User not found\r\n");
    
    channel = server->getChannel(channel_name);
    if (!channel)
        throw NoSuchChannel();
    if (!channel->hasClient(inviter) || !channel->isOperator(inviter))
        throw IRCException(":server 482 " + channel_name + " :Invalid channel name\r\n");

    channel->addInvite(target);
    
    msg = ":" + inviter->getNickname() + " INVITE " + target_nick + " :" + channel_name + "\r\n";
    send(target->getSocket(), msg.c_str(), msg.size(), 0);
}
