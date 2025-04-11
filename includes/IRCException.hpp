/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:52:19 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/10 11:30:13 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCEXCEPTION_HPP
# define IRCEXCEPTION_HPP

# include <stdexcept>
# include <string>

class IRCException : public std::runtime_error
{
    public:
        IRCException( int, const std::string& );
};

// ERR_ALREADYREGISTERED (462)
class AlreadyRegisteredException : public IRCException
{
    public:
        AlreadyRegisteredException( int );
};

// ERR_NEEDMOREPARAMS (461)
class NeedMoreParamsException : public IRCException
{
    public:
        NeedMoreParamsException( int, const std::string& );
};

// ERR_PASSWDMISMATCH (464)
class PasswordMismatchException : public IRCException
{
    public:
        PasswordMismatchException( int );
};

class NicknameInUse : public IRCException
{
    public:
        NicknameInUse( int, const std::string& );
};

class InvalidChannelNameException : public IRCException
{
    public:
        InvalidChannelNameException( int );
};

class NotOnChannelException : public IRCException
{
    public:
        NotOnChannelException( int, const std::string& );
};

#endif