/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdNick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:51:39 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/23 22:38:45 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"
#include <set>

void	CommandHandler::cmdNick( int client_socket, const std::string& nickname )
{
	if (!server->getClient(client_socket)->isAuthenticated())
		throw PasswordMismatchException();

	if (nickname.empty())
		throw NeedMoreParamsException("NICK");
	if (nickname.length() > 16)
		throw ErroneusNickname(nickname);

	std::vector<const Client*>  clients = server->getListClients();
	std::string                 response;

	for (std::vector<const Client*>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it)->getNickname() == nickname)
			throw NicknameInUse(nickname);
	}

	if (server->getClient(client_socket)->getNickname().empty())
	{
		server->getClient(client_socket)->setNickname(nickname);
		if (!server->getClient(client_socket)->getUsername().empty())
		{
			response = ":ft_irc 001 " + nickname + " :Welcome to the IRC Server\r\n";
			send(client_socket, response.c_str(), response.size(), 0);
		}
	}
	else
	{
		response = ":" + server->getClient(client_socket)->getNickname() + " NICK " + nickname + "\r\n";
		server->getClient(client_socket)->setNickname(nickname);

		std::set<int>	notifiedSockets;

		for (std::map<std::string, Channel*>::const_iterator it = server->getChannels().begin(); it != server->getChannels().end(); it++)
		{
			if (it->second->hasClient(server->getClient(client_socket)))
			{
				const std::vector<Client*>&	members = it->second->getMembers();
				for (size_t i = 0; i < members.size(); i++)
				{
					int sock = members[i]->getSocket();
					if (notifiedSockets.find(sock) == notifiedSockets.end())
					{
						send(sock, response.c_str(), response.size(), 0);
						notifiedSockets.insert(sock);
					}
				}
			}
		}
	}
}

