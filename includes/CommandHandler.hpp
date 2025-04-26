/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:22:33 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/26 10:08:10 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

# include "IRCServer.hpp"

class IRCServer;
class Channel;
class Client;

class CommandHandler
{
	private:
		typedef void    (CommandHandler::*CommandFunction)(int client_socket, const std::string& params);

		std::map<std::string, CommandFunction>	commands;
		IRCServer*								server;
		
		void	cmdJoin( int, const std::string& );
		void	cmdKick( int, const std::string& );
		void	cmdMode( int, const std::string& );
		void	cmdTopic( int, const std::string& );
		void	cmdPart( int, const std::string& );

		void	cmdPrivmsg( int, const std::string& );
		void	cmdPing( int, const std::string& );
		void	cmdWho( int, const std::string& );
		void	cmdNames( int, const std::string& );
		
		void	cmdInvite( int, const std::string& );
		void	cmdNick( int, const std::string& );
		void	cmdPass( int, const std::string& );
		void	cmdUser( int, const std::string& );

		void	cmdIgnored( int, const std::string& );
		
		void	handleChannelMessage( int, const std::string&, const std::string& );
		void	handlePrivateMessage( int, const std::string&, const std::string& );
		void	broadcastToChannel( Channel*, const std::string& );
		void	sendNumericReply( int, int, const std::string& );
		void	sendNamesList( Channel*, Client* );
		
	public:
		CommandHandler( IRCServer* );
		void	handleCommand( int, const std::string& );
};

#endif