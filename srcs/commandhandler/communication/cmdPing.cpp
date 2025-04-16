/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdPing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:52:42 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/16 14:55:24 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

void    CommandHandler::cmdPing( int client_socket, const std::string& param )
{
	if (param.empty())
		throw NeedMoreParamsException("PING");

	std::string response;
	
    response = ":server PONG " + param + "\r\n";
	send(client_socket, response.c_str(), response.size(), 0);
}