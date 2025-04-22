/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:29:16 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/22 14:47:53 by rrichard         ###   ########.fr       */
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
	commands["NAMES"] = &CommandHandler::cmdNames;
	commands["PART"] = &CommandHandler::cmdPart;
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
		{
			std::cout << "Client not registered\n";
			server->closeClientConnection(client_socket);
		}
	}
}

void	CommandHandler::broadcastToChannel( Channel* channel, const std::string& msg )
{
	const std::vector<Client*>&	members = channel->getMembers();

	for (std::vector<Client*>::const_iterator it = members.begin(); it != members.end(); it++)
			send((*it)->getSocket(), msg.c_str(), msg.size(), 0);
}

void	CommandHandler::sendNumericReply( int socket, int numeric, const std::string& target )
{
	std::ostringstream oss;
	
	switch (numeric)
	{
		case 1:
			oss << ":ft_irc 001 " << target << " :Welcome to the ft_irc server\r\n";
			break;
		case 2:
			oss << ":ft_irc 002 " << target << " :Your host is localhost\r\n";
			break;
		case 3:
		{
			time_t	now = time(NULL);
			char	buf[64];
			std::strftime(buf, sizeof(buf), "%a %b %d %Y at %H:%M:%S", std::localtime(&now));
			oss << ":ft_irc 003 " << target << " :This server was created " << buf << "\r\n";
			break;
		}
		case 4:
			oss << ":ft_irc 004 " << target << " ft_irc 1.0 o itklo\r\n";
			break;
		case 5:
			oss << ":ft_irc 005 " << target << " CHANMODES=itklo NICKLEN=16 CHANNELLEN=32 :are supported by this server\r\n";
			break;
	}
	std::string	reply = oss.str();
	send(socket, reply.c_str(), reply.size(), 0);
}