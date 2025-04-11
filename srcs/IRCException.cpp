/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:25:57 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/10 11:40:00 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCException.hpp"
#include "IRCServer.hpp"

IRCException::IRCException( int socket, const std::string& message ) : std::runtime_error(message) { (void)socket; }

AlreadyRegisteredException::AlreadyRegisteredException( int socket ) : IRCException(socket, ":server 462 * :You may not reregister\r\n") {}

NeedMoreParamsException::NeedMoreParamsException( int socket, const std::string& command ) : IRCException(socket, ":server 461 * " + command + " :Not enough parameters\r\n") {}

PasswordMismatchException::PasswordMismatchException( int socket ) : IRCException(socket, ":server 464 * :Bad password\r\n") {}

NicknameInUse::NicknameInUse( int socket, const std::string& nickname ) : IRCException(socket, std::string(":server 433 *") + nickname + std::string(" :Nickname is already in use\r\n")) {}

InvalidChannelNameException::InvalidChannelNameException( int socket ) : IRCException(socket, "Error: Invalid channel name\r\n") {}

NotOnChannelException::NotOnChannelException( int socket, const std::string& channel ) : IRCException(socket, ":server 442 " + channel + " :You're not on that channel\r\n") {} 
