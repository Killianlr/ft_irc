/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:52:19 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/16 16:03:17 by rrichard42       ###   ########.fr       */
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

// ERR_NOSUCHCHANNEL (403)
class NoSuchChannel : public IRCException
{
    public:
        NoSuchChannel();
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

#endif