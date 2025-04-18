/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:25:57 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/18 14:54:00 by rrichard         ###   ########.fr       */
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

NoSuchNick::NoSuchNick( const std::string& nick ) : IRCException(":server 401 " + nick + " :No such nickname\r\n") {}

NotOnChannelException::NotOnChannelException( const std::string& channel ) : IRCException(":server 442 * " + channel + " :You're not on that channel\r\n") {} 

ChanOPrivsNeeded::ChanOPrivsNeeded( const std::string& channel ) : IRCException(":server 482 " + channel + " :You're not channel operator\r\n") {}

InviteOnlyChan::InviteOnlyChan( const std::string& channel ) : IRCException(":server 473 " + channel + " :You're not invited to this channel\r\n") {}

UserNotInChannel::UserNotInChannel( const std::string& nick, const std::string& channel ) : IRCException(":server 441 " + nick + " " + channel + " :They aren't on that channel\r\n") {}

InvalidModeParam::InvalidModeParam( const std::string& target, char modeChar, const std::string& param ) : IRCException(":server 696 " + target + " " + modeChar + " " + param + " :Invalid parameter\r\n") {}

UModeUnkownFlag::UModeUnkownFlag() : IRCException(":server 501 * :Unkown MODE flag\r\n") {}

ChannelIsFull::ChannelIsFull( const std::string& channel ) : IRCException(":server 471 " + channel + " :Cannot join channel (+l)\r\n") {}

BadChannelKey::BadChannelKey( const std::string& channel ) : IRCException(":server 475 " + channel + " :Cannot join channel (+k)\r\n") {}

