/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdPass.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:51:37 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/16 14:55:55 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

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
