/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:25:57 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/16 16:45:20 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCException.hpp"
#include "IRCServer.hpp"

IRCException::IRCException( const std::string& message ) : std::runtime_error(message) {}

AlreadyRegisteredException::AlreadyRegisteredException() : IRCException(":server 462 * :You may not reregister\r\n") {}

NeedMoreParamsException::NeedMoreParamsException( const std::string& command ) : IRCException(":server 461 * " + command + " :Not enough parameters\r\n") {}

PasswordMismatchException::PasswordMismatchException() : IRCException(":server 464 * :Bad password\r\n") {}

NicknameInUse::NicknameInUse( const std::string& nickname ) : IRCException(":server 433 * " + nickname + " :Nickname is already in use\r\n") {}

NoSuchChannel::NoSuchChannel() : IRCException(":server 403 * :Invalid channel name\r\n") {}

NotOnChannelException::NotOnChannelException( const std::string& channel ) : IRCException(":server 442 * " + channel + " :You're not on that channel\r\n") {} 

ChanOPrivsNeeded::ChanOPrivsNeeded( const std::string& channel ) : IRCException(":server 482 " + channel + " :You're not channel operator\r\n") {}

InviteOnlyChan::InviteOnlyChan( const std::string& channel ) : IRCException(":server 473 " + channel + " :You're not invited to this channel\r\n") {}

UserNotInChannel::UserNotInChannel( const std::string& nick, const std::string& channel ) : IRCException(":server 441 " + nick + " " + channel + " :They aren't on that channel\r\n") {}