/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:52:19 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/21 22:43:57 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCEXCEPTION_HPP
# define IRCEXCEPTION_HPP

# include <stdexcept>
# include <string>

class IRCException : public std::runtime_error
{
    public:
        IRCException( const std::string& );
};

// ERR_ALREADYREGISTERED (462)
class AlreadyRegisteredException : public IRCException
{
    public:
        AlreadyRegisteredException();
};

// ERR_NEEDMOREPARAMS (461)
class NeedMoreParamsException : public IRCException
{
    public:
        NeedMoreParamsException( const std::string& );
};

// ERR_PASSWDMISMATCH (464)
class PasswordMismatchException : public IRCException
{
    public:
        PasswordMismatchException();
};

// ERR_NICKNAMEINUSE (433)
class NicknameInUse : public IRCException
{
    public:
        NicknameInUse( const std::string& );
};

// ERR_NOSUCHNICK (401)
class NoSuchNick : public IRCException
{
	public:
		NoSuchNick( const std::string& );
};

// ERR_NOSUCHCHANNEL (403)
class NoSuchChannel : public IRCException
{
    public:
        NoSuchChannel();
};

// ERR_ERRONEUSNICKNAME (432)
class ErroneusNickname : public IRCException
{
	public:
		ErroneusNickname( const std::string& );
};

// ERR_NOTONCHANNEL (442)
class NotOnChannelException : public IRCException
{
    public:
        NotOnChannelException( const std::string& );
};

// ERR_CHANOPRIVSNEEDED (482)
class ChanOPrivsNeeded : public IRCException
{
    public:
        ChanOPrivsNeeded( const std::string& );
};

// ERR_INVITEONLYCHAN (473)
class InviteOnlyChan : public IRCException
{
    public:
        InviteOnlyChan( const std::string& );
};

// ERR_USERNOTINCHANNEL (441)
class UserNotInChannel : public IRCException
{
    public:
        UserNotInChannel( const std::string&, const std::string& );
};

// ERR_UMODEUNKOWNFLAG (501)
class UModeUnkownFlag : public IRCException
{
	public:
		UModeUnkownFlag();
};

// ERR_INVALIDMODEPARAM (696)
class InvalidModeParam : public IRCException
{
	public:
		InvalidModeParam( const std::string&, char, const std::string& );
};

// ERR_CHANNELISFULL (471)
class ChannelIsFull : public IRCException
{
	public:
		ChannelIsFull( const std::string& );
};

// ERR_BADCHANNELKEY (475)
class BadChannelKey : public IRCException
{
	public:
		BadChannelKey( const std::string& );
};

#endif