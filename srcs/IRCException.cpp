/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:25:57 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/11 10:19:36 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCException.hpp"
#include "IRCServer.hpp"

IRCException::IRCException( const std::string& message ) : std::runtime_error(message) {}

AlreadyRegisteredException::AlreadyRegisteredException() : IRCException(":server 462 * :You may not reregister\r\n") {}

NeedMoreParamsException::NeedMoreParamsException( const std::string& command ) : IRCException(":server 461 * " + command + " :Not enough parameters\r\n") {}

PasswordMismatchException::PasswordMismatchException() : IRCException(":server 464 * :Bad password\r\n") {}

NicknameInUse::NicknameInUse( const std::string& nickname ) : IRCException(":server 433 * " + nickname + " :Nickname is already in use\r\n") {}

InvalidChannelNameException::InvalidChannelNameException() : IRCException(":server 403 * :Invalid channel name\r\n") {}

NotOnChannelException::NotOnChannelException( const std::string& channel ) : IRCException(":server 442 * " + channel + " :You're not on that channel\r\n") {} 
