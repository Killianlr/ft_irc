/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdNick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:51:39 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/18 15:20:53 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

void	CommandHandler::cmdNick( int client_socket, const std::string& nickname )
{
	if (!server->getClient(client_socket)->isAuthenticated())
		throw PasswordMismatchException();

	std::vector<const Client*>  clients = server->getListClients();
	std::string                 response;

	for (std::vector<const Client*>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it)->getNickname() == nickname)
			throw NicknameInUse(nickname);
	}
	
	server->getClient(client_socket)->setNickname(nickname);
	if (!server->getClient(client_socket)->getUsername().empty())
	{
		response = ":ft_irc 001 " + nickname + " :Welcome to the IRC Server\r\n";
		send(client_socket, response.c_str(), response.size(), 0);
	}
}

