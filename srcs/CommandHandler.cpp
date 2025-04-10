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
#include "Channel.hpp"

CommandHandler::CommandHandler( IRCServer* server ) : server(server)
{
    commands["NICK"] = &CommandHandler::cmdNick;
	commands["USER"] = &CommandHandler::cmdUser;
	commands["JOIN"] = &CommandHandler::cmdJoin;
	// commands["PRIVMSG"] = &CommandHandler::cmdPrivmsg;
	commands["PING"] = &CommandHandler::cmdPing;
    commands["PASS"] = &CommandHandler::cmdPass;

    commands["INVITE"] = &CommandHandler::cmdInvite;
    commands["KICK"] = &CommandHandler::cmdKick;

    commands["WHO"] = &CommandHandler::cmdIgnored;
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

void    CommandHandler::cmdJoin(int client_socket, const std::string& param)
{
    std::string response;
    std::string channel_name = param;
    Client* client = server->getClient(client_socket);
    Channel* channel;

    if (param.empty() || param[0] != '#')
        throw InvalidChannelNameException();
    if (server->getChannel(channel_name))
    {
        std::cout << "Channel : " << channel_name << " already exist" << std::endl;
        channel = server->getChannel(channel_name);
    }
    else
    {
        std::cout << "Channel : " << channel_name << " be created" << std::endl;
        channel = new Channel(channel_name);
        server->addChannel(channel_name, channel);
        client->setOperators();
    }
    channel->addClient(client);

    response = ":" + client->getNickname() + " JOIN " + channel_name + "\r\n";
    send(client_socket, response.c_str(), response.size(), 0);
}

/*########################################################################################
#                                    CHANNEL COMMAND                                     #
########################################################################################*/

void    CommandHandler::cmdKick(int client_socket, const std::string& param)
{
    std::istringstream  iss(param);
    std::string         channel_name, target_nick, reason;
    Channel*            channel;
    Client*             kicker;
    Client*             target;
    std::string         msg;

    iss >> channel_name >> target_nick >> reason;
    if (reason.empty())
        reason = "no particular reason";

    if (channel_name.empty() || target_nick.empty())
        throw IRCException("Error: Kick syntax invalid\r\n");
    
    channel = server->getChannel(channel_name);
    if (!channel)
        throw IRCException("Error: Channel not found\r\n");
    
    kicker = server->getClient(client_socket);
    if (!channel->hasClient(kicker) || !channel->isOperator(kicker))
        throw IRCException("Error: You are not channel operator\r\n");
    
    target = server->getClientByNickName(target_nick);
    if (!target || !channel->hasClient(target))
        throw IRCException("Error: User not in channel\r\n");

    channel->removeClient(target);

    msg = ":" + kicker->getNickname() +
                " KICK " + channel_name + " " + target_nick +
                " :" + reason + "\r\n";
    
    const std::vector<Client*>& channel_members = channel->getMembers();
    for (size_t i = 0; i < channel_members.size(); ++i)
        send(channel_members[i]->getSocket(), msg.c_str(), msg.size(), 0);
}

void    CommandHandler::cmdInvite(int client_socket, const std::string& param)
{
    std::istringstream  iss(param);
    std::string         target_nick, channel_name;
    Client*             inviter;
    Client*             target;
    Channel*            channel;
    std::string         msg;

    iss >> target_nick >> channel_name;
    if (target_nick.empty() || channel_name.empty())
        throw IRCException("Error: INVITE syntax invalid\r\n");
    
    inviter = server->getClient(client_socket);
    target  = server->getClientByNickName(target_nick);
    if (!target)
        throw IRCException("Error: User not found\r\n");
    
    channel = server->getChannel(channel_name);
    if (!channel)
        throw IRCException("Error: Channel not found\r\n");
    if (!channel->hasClient(inviter) || !channel->isOperator(inviter))
        throw IRCException("Error: You're not operator of the channel\r\n");

    channel->addInvite(target);
    
    msg = ":" + inviter->getNickname() + " INVITE " + target_nick + " :" + channel_name + "\r\n";
    send(target->getSocket(), msg.c_str(), msg.size(), 0);
}

void    CommandHandler::cmdIgnored( int client_socket, const std::string& param)
{
    (void)client_socket;
    (void)param;
    std::cout << ": [Command ignored]" << std::endl;
}