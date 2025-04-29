/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdNick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:51:39 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/29 15:50:55 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"
#include <set>

void	CommandHandler::cmdNick( int client_socket, const std::string& nickname )
{
	Client*	client = server->getClient(client_socket);

	if (!client->isAuthenticated())
		throw PasswordMismatchException();

	if (nickname.empty())
		throw NeedMoreParamsException("NICK");
	if (nickname.length() > 16)
		throw ErroneusNickname(nickname);
	if (nickname[0] == '#' || nickname[0] == ':')	
		throw ErroneusNickname("<invalid>");

	std::vector<const Client*>  clients = server->getListClients();
	std::string                 response;

	for (std::vector<const Client*>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it)->getNickname() == nickname)
			throw NicknameInUse(nickname);
	}
	
	client->setNickname(nickname);
	if (!client->isRegistered())
	{
		client->setRegistered();
		sendNumericReply(client_socket, 1, client->getNickname());
		sendNumericReply(client_socket, 2, client->getNickname());
		sendNumericReply(client_socket, 3, client->getNickname());
		sendNumericReply(client_socket, 4, client->getNickname());
		sendNumericReply(client_socket, 5, client->getNickname());
		server->getChannel("#general")->addClient(client);
		return ;
	}
	response = ":" + client->getNickname() + " NICK " + nickname + "\r\n";

	std::set<int>	notifiedSockets;

	const std::vector<Channel*>&	channels = client->getChannels();
	for (size_t i = 0; i < channels.size(); i++)
	{
		const std::vector<Client*>&	members = channels[i]->getMembers();
		for (size_t j = 0; j < members.size(); j++)
		{
			int sock = members[j]->getSocket();
			if (notifiedSockets.find(sock) == notifiedSockets.end())
			{
				send(sock, response.c_str(), response.size(), 0);
				notifiedSockets.insert(sock);
			}
		}
	}
	if (notifiedSockets.find(client_socket) == notifiedSockets.end())
		send(client_socket, response.c_str(), response.size(), 0);
}

