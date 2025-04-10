/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:22:33 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/04 13:49:13 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

# include "IRCServer.hpp"

class IRCServer;

class CommandHandler
{
	private:
		typedef void    (CommandHandler::*CommandFunction)(int client_socket, const std::string& params);

		std::map<std::string, CommandFunction>	commands;
		IRCServer*								server;
	
		void	cmdNick( int, const std::string& );
		void	cmdUser( int, const std::string& );
		void	cmdJoin( int, const std::string& );
		void	cmdPrivmsg( int, const std::string& );
		void	cmdPing( int, const std::string& );
		void	cmdPass( int, const std::string& );

		void	cmdKick( int, const std::string& );
		void	cmdInvite( int, const std::string& );
		void	cmdIgnored( int, const std::string& );
		
		void	sendRegistrationMessages( int, const std::string& );
		
	public:
		CommandHandler( IRCServer* );
		void	handleCommand( int, const std::string& );
};

#endif