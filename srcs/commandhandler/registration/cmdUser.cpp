/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdUser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:51:32 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/16 14:52:19 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

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
