/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:29:16 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/11 09:05:22 by rrichard         ###   ########.fr       */
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
		// server->closeClientConnection(client_socket);
	}
}
#include <stdio.h>
void	CommandHandler::cmdNick( int client_socket, const std::string& nickname )
{
	if (!server->getClient(client_socket)->isAuthenticated())
		throw PasswordMismatchException(client_socket);

	std::vector<const Client*>  clients = server->getListClients();
	std::string                 response;

	for (std::vector<const Client*>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it)->getNickname() == nickname)
			throw NicknameInUse(client_socket, nickname);
	}
	
	server->getClient(client_socket)->setNickname(nickname);
	std::cout << "Nickname is set to: " + server->getClient(client_socket)->getNickname() << std::endl;
	if (!server->getClient(client_socket)->getUsername().empty())
	{
		response = ":server 001 " + nickname + " :Welcome to the IRC Server\r\n";
		send(client_socket, response.c_str(), response.size(), 0);
	}
}

void	CommandHandler::cmdUser( int client_socket, const std::string& userInfo )
{
	if (!server->getClient(client_socket)->isAuthenticated())
		throw PasswordMismatchException(client_socket);
	if (server->getClient(client_socket)->isRegistered())
		throw AlreadyRegisteredException(client_socket);

	std::vector<std::string>    tokens;
	std::string                 response, token;
	size_t                      pos = 0, start = 0;

	while ((pos = userInfo.find(' ', start)) != std::string::npos)
	{
		token = userInfo.substr(start, pos - start);
		if (!token.empty())
			tokens.push_back(token);
		start = pos + 1;
		if (userInfo[start] == ':')
		{
			tokens.push_back(userInfo.substr(start + 1));
			break ;
		}
	}
	if (start < userInfo.size() && userInfo[start] != ':')
		tokens.push_back(userInfo.substr(start));
	
	if (tokens.size() < 4)
		throw NeedMoreParamsException(client_socket, "USER");

	std::string username = tokens[0];
	std::string hostname = (tokens.size() > 1 ? tokens[1] : "0");
	std::string servername = (tokens.size() > 1 ? tokens[2] : "*");
	std::string realname = tokens[3];

	Client& client = *server->getClient(client_socket);
	client.setUsername(username);
	client.setRealname(realname);
	if (!client.getNickname().empty())
	{
		response = ":server 001 " + client.getNickname() + " :Welcome to the IRC Server\r\n";
		send(client_socket, response.c_str(), response.size(), 0);
	}
}

void    CommandHandler::cmdPing( int client_socket, const std::string& param )
{
	if (param.empty())
		throw NeedMoreParamsException(client_socket, "PING");

	std::string response;
	
	response = "PONG " + server->getServerName() + " " + param + "\r\n";
	send(client_socket, response.c_str(), response.size(), 0);
}

void    CommandHandler::cmdPass( int client_socket, const std::string& password )
{
	std::string response;
	Client&     client = *server->getClient(client_socket);

	if (client.isAuthenticated() == true)
		throw AlreadyRegisteredException(client_socket);
	if (password == "")
		throw NeedMoreParamsException(client_socket, "PASS");
	if (password != server->getPassword())
		throw PasswordMismatchException(client_socket);

	client.setAuthenticated();
	response = ":server NOTICE * :Password accepted\r\n";
	send(client_socket, response.c_str(), response.size(), 0);
}

void    CommandHandler::cmdJoin(int client_socket, const std::string& param)
{
	std::string response;
	std::string channel_name = param;
	Client* client = server->getClient(client_socket);
	Channel* channel;

	if (param.empty() || param[0] != '#')
		throw InvalidChannelNameException(client_socket);
	if (server->getChannel(channel_name))
		channel = server->getChannel(channel_name);
	else
	{
		channel = new Channel(channel_name);
		server->addChannel(channel_name, channel);
	}
	channel->addClient(client);

	response = ":" + client->getNickname() + " JOIN " + channel_name + "\r\n";
	send(client_socket, response.c_str(), response.size(), 0);
}

void    CommandHandler::cmdPrivmsg( int client_socket, const std::string& param )
{
	std::istringstream  iss(param);
	std::string         target, message, response;

	if (!(iss >> target))
		throw NeedMoreParamsException(client_socket, "PRIVMSG");
	
	std::getline(iss >> std::ws, message);
	if (target[0] == '#' || target[0] == '&')
		handleChannelMessage(client_socket, target, message);
	else
		handlePrivateMessage(client_socket, target, message);
}

void    CommandHandler::handleChannelMessage( int client_socket, const std::string& channel, const std::string& message )
{
	if (!server->isClientInChannel(client_socket, channel))
		throw NotOnChannelException(client_socket, channel);

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
	{
		std::string response = ":server 401 * " + target + " :No such nick/channel\r\n";
		send(client_socket, response.c_str(), response.size(), 0);
		return ;
	}

	Client*	sender = server->getClient(client_socket);
	Client*	receiver = server->getClientByNickname(target);
	std::string	response = ":" + sender->getNickname() + " PRIVMSG " + target + " :" + message + "\r\n";
	send(receiver->getSocket(), response.c_str(), response.size(), 0);
}