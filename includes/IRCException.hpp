/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:52:19 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/04 15:58:31 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCEXCEPTION_HPP
# define IRCEXCEPTION_HPP

# include <stdexcept>
# include <string>

class IRCException : public std::runtime_error
{
    public:
        IRCException( const std::string& message ) : std::runtime_error(message) {}
};

// ERR_ALREADYREGISTERED (462)
class AlreadyRegisteredException : public IRCException
{
    public:
        AlreadyRegisteredException() : IRCException(":server 462 * :You may not reregister\n\r") {}
};

// ERR_NEEDMOREPARAMS (461)
class NeedMoreParamsException : public IRCException
{
    public:
        NeedMoreParamsException( const std::string& command ) : IRCException(":server 461 * " + command + " :Not enough parameters\r\n") {}
};

// ERR_PASSWDMISMATCH (464)
class PasswordMismatchException : public IRCException
{
    public:
        PasswordMismatchException() : IRCException(":server 464 * :Bad password\r\n") {}
};

class NicknameInUse : public IRCException
{
    public:
        NicknameInUse( const std::string& nickname ) : IRCException(":server 433 * " + nickname + " :Nickname is already in use") {}
};

class InvalidChannelNameException : public IRCException
{
    public:
        InvalidChannelNameException() : IRCException("Error: Invalid channel name\r\n") {}
};

#endif