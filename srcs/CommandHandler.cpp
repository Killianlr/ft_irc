/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:29:16 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/15 12:39:37 by rrichard42       ###   ########.fr       */
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
		throw PasswordMismatchException();
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
		client.setRegistered();
		send(client_socket, response.c_str(), response.size(), 0);
	}
}

void    CommandHandler::cmdPing( int client_socket, const std::string& param )
{
	if (param.empty())
		throw NeedMoreParamsException("PING");

	std::string response;
	
	response = "PONG server " + param + "\r\n";
	send(client_socket, response.c_str(), response.size(), 0);
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
    std::istringstream  iss(param);
    std::string response;
    std::string channel_name, key;
    iss >> channel_name >> key;
    Client* client = server->getClient(client_socket);
    Channel* channel = server->getChannel(channel_name);

    if (param.empty() || param[0] != '#')
        throw InvalidChannelNameException();
    if (channel)
    {
        // std::cout << "Channel : " << channel_name << " already exist" << std::endl;
        // channel = server->getChannel(channel_name);
        // if (channel->getNbMembers() > )
        if (channel->getKey() != key)
        {
            std::cout << "Wrong password\n" << std::endl;
            return ;
        }
        if (channel->isInviteOnly() && !channel->isInvite(client))
        {
            std::cout << "You're not invited to this channel\n" << std::endl; // MUST BE A EXCEPTION
            return ;
        }
        if (channel->getUserLimit() > 0 && channel->getNbMembers() >= channel->getUserLimit())
        {
            std::cout << "Channel is full\n" << std::endl;
            return ;
        }
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
    
    target = server->getClientByNickname(target_nick);
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
        throw NeedMoreParamsException("INVITE");
    
    inviter = server->getClient(client_socket);
    target  = server->getClientByNickname(target_nick);
    if (!target)
        throw IRCException("Error: User not found\r\n");
    
    channel = server->getChannel(channel_name);
    if (!channel)
        throw InvalidChannelNameException();
    if (!channel->hasClient(inviter) || !channel->isOperator(inviter))
        throw IRCException(":server 482 " + channel_name + " :Invalid channel name\r\n");

    channel->addInvite(target);
    
    msg = ":" + inviter->getNickname() + " INVITE " + target_nick + " :" + channel_name + "\r\n";
    send(target->getSocket(), msg.c_str(), msg.size(), 0);
}

void    CommandHandler::cmdMode(int client_socket, const std::string &param)
{
    std::istringstream  iss(param);
    std::string         channel_name, modes, params, msg;
    Client*             target;
    Channel*            channel;
    bool                adding = true;

    iss >> channel_name >> modes >> params;
    if (param[0] != '#')
    {
        std::cout << "User MODE command received: ignoring for now" << std::endl;
        return ;
    }
    
    if (channel_name.empty() || modes.empty())
    {
        std::cout << "Error: MODE syntax invalid" << std::endl;
        throw IRCException("Error: MODE syntax invalid\r\n");
    }
    channel = server->getChannel(channel_name);
    if (!channel)
    {
        std::cout << "Error: MODE Channel not found" << std::endl;
        throw IRCException("Error: MODE Channel not found\r\n");
    }

    if (!channel->isOperator(server->getClient(client_socket)))
    {
        std::cout << "Error: you arent an operator\n" << std::endl;
        return ;
    }
    for (size_t i = 0; i < modes.length(); ++i)
    {
        if (modes[i] == '+') adding = true;
        else if (modes[i] == '-') adding = false;
        else if (modes[i] == 'i') channel->setInviteOnly(adding);
        else if (modes[i] == 't') channel->setTopicRestricted(adding);
        else if (modes[i] == 'k')
        {
            if (adding)
            {
                if (params.empty())
                {
                    std::cout << "Error: MODE +k missing key" << std::endl;
                    throw IRCException("Error: MODE +k missing key\r\n");
                }
                channel->setKey(params);
            }
            else
                channel->setKey("");
        }
        else if (modes[i] == 'l')
        {
            if (adding)
            {
                if (params.empty())
                {
                    std::cout << "Error: MODE +l missing limit value" << std::endl;
                    throw IRCException("Error: MODE +l missing limit value\r\n");
                }
                channel->setUserLimit(atoi(params.c_str()));
            }
            else
                channel->setUserLimit(-1);
        }
        else if (modes[i] == 'o')
        {
            if (params.empty())
            {
                std::cout << "Error: MODE +o missing nickname" << std::endl;
                throw IRCException("Error: MODE +o missing nickname\r\n");
            }
            target = server->getClientByNickname(params);
            if (!target)
            {
                std::cout << "Error: MODE +o no such nickname" << std::endl;
                throw IRCException("Error: MODE +o no such nickname\r\n");
            }
            if (adding)
                channel->setOperator(target, true);
            else
                channel->setOperator(target, false);
        }
        else
        {
            std::cout << "Error: Unknown mode" << std::endl;
            throw IRCException("Error: Unknown mode\r\n");
        }
    }
    msg = ":" + (server->getClient(client_socket))->getNickname() + " MODE #" + channel_name + " " + modes + "\r\n";
    
    const std::vector<Client*>& channel_members = channel->getMembers();
    for (size_t i = 0; i < channel_members.size(); ++i)
        send(channel_members[i]->getSocket(), msg.c_str(), msg.size(), 0);
}

void    CommandHandler::cmdIgnored( int client_socket, const std::string& param)
{
    (void)client_socket;
    (void)param;
    std::cout << ": [Command ignored]" << std::endl;
}

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