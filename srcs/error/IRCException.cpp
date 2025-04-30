/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:25:57 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/30 18:16:37 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCException.hpp"
#include "IRCServer.hpp"

IRCException::IRCException( const std::string& message ) : std::runtime_error(message) {}

NoSuchNick::NoSuchNick( const std::string& nick ) : IRCException(":ft_irc 401 " + nick + " :No such nickname\r\n") {}

NoSuchChannel::NoSuchChannel() : IRCException(":ft_irc 403 * :Invalid channel name\r\n") {}

NicknameInUse::NicknameInUse( const std::string& nickname ) : IRCException(":ft_irc 433 " + nickname + " :Nickname is already in use\r\n") {}

NeedMoreParamsException::NeedMoreParamsException( const std::string& command ) : IRCException(":ft_irc 461 " + command + " :Not enough parameters\r\n") {}

AlreadyRegisteredException::AlreadyRegisteredException() : IRCException(":ft_irc 462 * :You may not reregister\r\n") {}

PasswordMismatchException::PasswordMismatchException() : IRCException(":ft_irc 464 * :Bad password\r\n") {}

UserNotInChannel::UserNotInChannel( const std::string& nick, const std::string& channel ) : IRCException(":ft_irc 441 " + nick + " " + channel + " :They aren't on that channel\r\n") {}

ErroneusNickname::ErroneusNickname( const std::string& nickname ) : IRCException(":ft_irc 432 " + nickname + " :Erroneus nickname\r\n") {}

NotOnChannelException::NotOnChannelException( const std::string& channel ) : IRCException(":ft_irc 442 " + channel + " :You're not on that channel\r\n") {} 

NotRegistered::NotRegistered() : IRCException(":ft_irc 451 * :You have not registered\r\n") {}

ChannelIsFull::ChannelIsFull( const std::string& channel ) : IRCException(":ft_irc 471 " + channel + " :Cannot join channel (+l)\r\n") {}

InviteOnlyChan::InviteOnlyChan( const std::string& channel ) : IRCException(":ft_irc 473 " + channel + " :You're not invited to this channel\r\n") {}

BadChannelKey::BadChannelKey( const std::string& channel ) : IRCException(":ft_irc 475 " + channel + " :Cannot join channel (+k)\r\n") {}

ChanOPrivsNeeded::ChanOPrivsNeeded( const std::string& channel ) : IRCException(":ft_irc 482 " + channel + " :You're not channel operator\r\n") {}

UModeUnkownFlag::UModeUnkownFlag() : IRCException(":ft_irc 501 * :Unkown MODE flag\r\n") {}

InvalidModeParam::InvalidModeParam( const std::string& target, char modeChar, const std::string& param ) : IRCException(":ft_irc 696 " + target + " " + modeChar + " " + param + " :Invalid parameter\r\n") {}
