/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:44:15 by robincanava       #+#    #+#             */
/*   Updated: 2025/04/03 20:24:26 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

# include <iostream>
# include <cstdio>
# include <sstream>
# include <string>
# include <vector>
# include <poll.h>
# include <map>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <cstdlib>

typedef struct pollfd t_pollfd;

const int	POLL_TIMEOUT = 1000;

struct Client
{
	int			socket;
	std::string	nickname;
	std::string	username;
	bool		authenticated;
};

class IRCServer
{
	private:
		int												port;
		std::string										password;
		int												server_fd;
		std::vector<t_pollfd>							poll_fds;
		std::map<int, Client>							clients;
		std::map<std::string, void (IRCServer::*)( int, const std::string& )>	commands;

		IRCServer&	operator=( const IRCServer& );
		IRCServer( IRCServer& );

		void	setupServer();
		void	runEventLoop();
		void	handleNewConnection();
		void	handleClientData( int );
		void	handleCommand( int, const std::string& );

		void	cmdNick( int, const std::string& );
		void	cmdUser( int, const std::string& );
		void	cmdJoin( int, const std::string& );
		void	cmdPrivmsg( int, const std::string& );
		void	cmdPing( int, const std::string& );
	
	public:
		IRCServer( int, const std::string& );
		~IRCServer();

		void	start();
};

#endif