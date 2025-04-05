/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:29:16 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/05 15:43:10 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"

CommandHandler::CommandHandler( IRCServer* server ) : server(server)
{
    commands["NICK"] = &CommandHandler::cmdNick;
	commands["USER"] = &CommandHandler::cmdUser;
	// commands["JOIN"] = &CommandHandler::cmdJoin;
	// commands["PRIVMSG"] = &CommandHandler::cmdPrivmsg;
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
        send(client_socket, errorMsg.c_str(), errorMsg.size(), 0);
    }
}

void	CommandHandler::cmdNick( int client_socket, const std::string& nickname )
{
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
        response = ":server 001 " + nickname + " :Welcome to the IRC Server\r\n";
        send(client_socket, response.c_str(), response.size(), 0);
    }
}

void	CommandHandler::cmdUser( int client_socket, const std::string& userInfo )
{
    if (server->getClient(client_socket)->isRegistered())
        throw AlreadyRegisteredException();

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
        throw NeedMoreParamsException("USER");

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
    std::string response = "PONG " + param + "\r\n";
    write(client_socket, response.c_str(), response.size());
}

void    CommandHandler::cmdPass( int client_socket, const std::string& password )
{
    std::string response;
    Client&     client = *server->getClient(client_socket);

    if (client.isAuthenticated() == true)
        throw AlreadyRegisteredException();
    if (password == "")
        throw NeedMoreParamsException("PASS");
    if (password != server->getPassword())
        throw PasswordMismatchException();

    client.setAuthenticated();
    response = ":server NOTICE * :Password accepted\r\n";
    send(client_socket, response.c_str(), response.size(), 0);
}