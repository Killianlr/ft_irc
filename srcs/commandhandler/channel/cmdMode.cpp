/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:58:06 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/16 14:59:19 by rrichard42       ###   ########.fr       */
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
    bool                adding = true;

    iss >> channel_name >> modes >> params;
    if (param[0] != '#')
    {
        std::cout << "User MODE command received: ignoring for now" << std::endl;
        return ;
    }
    
    if (channel_name.empty() || modes.empty())
    {
        std::cout << "Error: MODE syntax invalid" << std::endl;
        throw IRCException("Error: MODE syntax invalid\r\n");
    }
    channel = server->getChannel(channel_name);
    if (!channel)
    {
        std::cout << "Error: MODE Channel not found" << std::endl;
        throw IRCException("Error: MODE Channel not found\r\n");
    }

    for (size_t i = 0; i < modes.length(); ++i)
    {
        if (modes[i] == '+') adding = true;
        else if (modes[i] == '-') adding = false;
        else if (modes[i] == 'i') channel->setInviteOnly(adding);
        else if (modes[i] == 't') channel->setTopicRestricted(adding);
        else if (modes[i] == 'k')
        {
            if (adding)
            {
                if (params.empty())
                {
                    std::cout << "Error: MODE +k missing key" << std::endl;
                    throw IRCException("Error: MODE +k missing key\r\n");
                }
                channel->setKey(params);
            }
            else
                channel->setKey("");
        }
        else if (modes[i] == 'l')
        {
            if (adding)
            {
                if (params.empty())
                {
                    std::cout << "Error: MODE +l missing limit value" << std::endl;
                    throw IRCException("Error: MODE +l missing limit value\r\n");
                }
                channel->setUserLimit(atoi(params.c_str()));
            }
            else
                channel->setUserLimit(-1);
        }
        else if (modes[i] == 'o')
        {
            if (params.empty())
            {
                std::cout << "Error: MODE +o missing nickname" << std::endl;
                throw IRCException("Error: MODE +o missing nickname\r\n");
            }
            target = server->getClientByNickname(params);
            if (!target)
            {
                std::cout << "Error: MODE +o no such nickname" << std::endl;
                throw IRCException("Error: MODE +o no such nickname\r\n");
            }
            if (adding)
                channel->setOperator(target, true);
            else
                channel->setOperator(target, false);
        }
        else
        {
            std::cout << "Error: Unknown mode" << std::endl;
            throw IRCException("Error: Unknown mode\r\n");
        }
    }
    msg = ":" + (server->getClient(client_socket))->getNickname() + " MODE #" + channel_name + " " + modes + "\r\n";
    
    const std::vector<Client*>& channel_members = channel->getMembers();
    for (size_t i = 0; i < channel_members.size(); ++i)
        send(channel_members[i]->getSocket(), msg.c_str(), msg.size(), 0);
}
