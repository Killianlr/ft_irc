/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdNick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:51:39 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/26 08:50:27 by rrichard         ###   ########.fr       */
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

	if (client->getNickname().empty())
	{
		server->getClient(client_socket)->setNickname(nickname);
		if (!server->getClient(client_socket)->getUsername().empty())
		{
			response = ":ft_irc 001 " + nickname + " :Welcome to the ft_irc Server\r\n";
			send(client_socket, response.c_str(), response.size(), 0);
			return ;
		}
	}
	response = ":" + client->getNickname() + " NICK " + nickname + "\r\n";
	client->setNickname(nickname);

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

