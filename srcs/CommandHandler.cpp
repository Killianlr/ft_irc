/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:29:16 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/04 14:28:31 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"

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
    std::cout << command << std::endl;
	if (commands.find(command) != commands.end())
		(this->*commands[command])(client_socket, param);
}

void	CommandHandler::cmdNick( int client_socket, const std::string& nickname )
{
    std::map<int, Client>   clients = server->getMapClient();
    std::string             response;

    for (std::map<int, Client>::const_iterator cit = clients.begin(); cit != clients.end(); cit++)
    {
        if (cit->second.nickname == nickname)
        {
            std::string response = ":server 433 * " + nickname + " :Nickname is already in use\r\n";
            send(client_socket, response.c_str(), response.size(), 0);
            return ;
        }
    }
    server->getClient(client_socket)->nickname = nickname;
    response = ":server 001 " + nickname + " :Welcome to the IRC Server\r\n";
    send(client_socket, response.c_str(), response.size(), 0);
}

void	CommandHandler::cmdUser( int client_socket, const std::string& userInfo )
{
    std::istringstream  iss(userInfo);
    std::string         username, hostname, servername, realname;

    if (!(iss >> username >> hostname >> servername))
    {
        std::string response = ":server 461 * USER : Not enough parameters\r\n";
        send(client_socket, response.c_str(), response.size(), 0);
        return ;
    }
    std::getline(iss, realname);
    if (!realname.empty() && realname[0] == ':')
        realname = realname.substr(1);
    server->getClient(client_socket)->username = username;
    server->getClient(client_socket)->realname = realname;
    std::string response = ":server 001 " + server->getClient(client_socket)->nickname + ": \r\n";
    send(client_socket, response.c_str(), response.size(), 0);
}

void    CommandHandler::cmdPing( int client_socket, const std::string& param )
{
    std::string response = "PONG " + param + "\r\n";
    write(client_socket, response.c_str(), response.size());
}

void    CommandHandler::cmdPass( int client_socket, const std::string& password )
{
    std::string response;

    if (server->getClient(client_socket)->authenticated == true) // ERR_ALREADYREGISTERED (462)
    {
        response = ":server 462 * :You may not reregister\r\n";
        send(client_socket, response.c_str(), response.size(), 0);
        std::cout << response;
    }
    else if (password == server->getPassword())
    {
        server->getClient(client_socket)->authenticated = true;
        response = ":server NOTICE * :*** Password accepted\r\n";
        send(client_socket, response.c_str(), response.size(), 0);
        std::cout << response;
    }
    else if (password == "") // ERR_NEEDMOREPARAMS (461)
    {
        response = ":server 461 * :Not enough parameters\r\n";
        send(client_socket, response.c_str(), response.size(), 0);
        std::cout << response;
    }
    else // ERR_PASSWDMISMATCH (464)
    {
        response = ":server 464 * :Bad password\r\n";
        send(client_socket, response.c_str(), response.size(), 0);
        std::cout << response;
        // close(client_socket);
        // server->removeClient(client_socket);
    }
}