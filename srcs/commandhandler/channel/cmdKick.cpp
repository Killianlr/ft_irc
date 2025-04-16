/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdKick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:58:18 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/16 14:58:48 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

void    CommandHandler::cmdKick(int client_socket, const std::string& param)
{
    std::istringstream  iss(param);
    std::string         channel_name, target_nick, reason;
    Channel*            channel;
    Client*             kicker;
    Client*             target;
    std::string         msg;

    iss >> channel_name >> target_nick >> reason;
    if (reason.empty())
        reason = "no particular reason";

    if (channel_name.empty() || target_nick.empty())
        throw IRCException("Error: Kick syntax invalid\r\n");
    
    channel = server->getChannel(channel_name);
    if (!channel)
        throw IRCException("Error: Channel not found\r\n");
    
    kicker = server->getClient(client_socket);
    if (!channel->hasClient(kicker) || !channel->isOperator(kicker))
        throw IRCException("Error: You are not channel operator\r\n");
    
    target = server->getClientByNickname(target_nick);
    if (!target || !channel->hasClient(target))
        throw IRCException("Error: User not in channel\r\n");

    channel->removeClient(target);

    msg = ":" + kicker->getNickname() +
                " KICK " + channel_name + " " + target_nick +
                " :" + reason + "\r\n";
    
    const std::vector<Client*>& channel_members = channel->getMembers();
    for (size_t i = 0; i < channel_members.size(); ++i)
        send(channel_members[i]->getSocket(), msg.c_str(), msg.size(), 0);
}
