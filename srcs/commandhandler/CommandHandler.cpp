/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:29:16 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/16 15:42:55 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

CommandHandler::CommandHandler( IRCServer* server ) : server(server)
{
	commands["NICK"] = &CommandHandler::cmdNick;
	commands["USER"] = &CommandHandler::cmdUser;
	commands["JOIN"] = &CommandHandler::cmdJoin;
	commands["PRIVMSG"] = &CommandHandler::cmdPrivmsg;
	commands["PING"] = &CommandHandler::cmdPing;
    commands["PASS"] = &CommandHandler::cmdPass;
    commands["INVITE"] = &CommandHandler::cmdInvite;
    commands["KICK"] = &CommandHandler::cmdKick;
    commands["MODE"] = &CommandHandler::cmdMode;
	commands["TOPIC"] = &CommandHandler::cmdTopic;
}

void	CommandHandler::handleCommand( int client_socket, const std::string& message )
{
	size_t		pos = message.find(' ');
	std::string	command = message.substr(0, pos);
	std::string	param;

	if (pos != std::string::npos)
		param = message.substr(pos + 1);
	else
		param = "";
	try
	{
		if (commands.find(command) != commands.end())
			(this->*commands[command])(client_socket, param);
		else
			std::cerr << "Unknown command from client " << std::endl;
	}
	catch (const IRCException &e)
	{
		std::string errorMsg = e.what();
		std::cout << errorMsg << std::endl;
		send(client_socket, errorMsg.c_str(), errorMsg.size(), 0);
		if (!server->getClient(client_socket)->isRegistered())
			server->closeClientConnection(client_socket);
	}
}

/*########################################################################################
#                                    CHANNEL COMMAND                                     #
########################################################################################*/

void    CommandHandler::cmdIgnored( int client_socket, const std::string& param)
{
    (void)client_socket;
    (void)param;
    std::cout << ": [Command ignored]" << std::endl;
}

void	CommandHandler::broadcastToChannel( Channel* channel, const std::string& msg, int exclude_socket )
{
	const std::vector<Client*>&	members = channel->getMembers();
	for (std::vector<Client*>::const_iterator it = members.begin(); it != members.end(); it++)
	{
		if ((*it)->getSocket() != exclude_socket)
			send((*it)->getSocket(), msg.c_str(), msg.size(), 0);
	}
}