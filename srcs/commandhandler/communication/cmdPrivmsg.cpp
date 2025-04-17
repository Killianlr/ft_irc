/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdPrivmsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:52:56 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/17 11:13:52 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

void    CommandHandler::cmdPrivmsg( int client_socket, const std::string& param )
{
	std::istringstream  iss(param);
	std::string         target, message, response;

	if (!(iss >> target))
		throw NeedMoreParamsException("PRIVMSG");
	
	std::getline(iss >> std::ws, message);
	if (target[0] == '#' || target[0] == '&')
		handleChannelMessage(client_socket, target, message);
	else
		handlePrivateMessage(client_socket, target, message);
}

void    CommandHandler::handleChannelMessage( int client_socket, const std::string& channel, const std::string& message )
{
	if (!server->isClientInChannel(client_socket, channel))
		throw NotOnChannelException(channel);

	Client*					sender = server->getClient(client_socket);
	std::vector<Client*>	clients = server->getClientsInChannel(channel);
	std::string				response = ":" + sender->getNickname() + " PRIVMSG " + channel + " :" + message + "\r\n";

	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it)->getSocket() != client_socket)
			send((*it)->getSocket(), response.c_str(), response.size(), 0);
	}
}

void	CommandHandler::handlePrivateMessage( int client_socket, const std::string& target, const std::string& message )
{
	if (!server->getClientByNickname(target))
		throw NoSuchChannel();

	Client*		sender = server->getClient(client_socket);
	Client*		receiver = server->getClientByNickname(target);
	std::string	response = ":" + sender->getNickname() + " PRIVMSG " + target + " :" + message + "\r\n";

	send(receiver->getSocket(), response.c_str(), response.size(), 0);
}