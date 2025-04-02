/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: robincanavaggio <robincanavaggio@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:44:15 by robincanava       #+#    #+#             */
/*   Updated: 2025/04/02 19:50:46 by robincanava      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <poll.h>
# include <map>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>

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
		int							port;
		std::string					password;
		int							server_fd;
		std::vector<t_pollfd>		poll_fds;
		std::map<int, Client>		clients;

		IRCServer&	operator=( const IRCServer& );
		IRCServer( IRCServer& );

		void	setupServer();
		void	runEventLoop();
		void	handleNewConnection();
		void	handleClientData( int );
	
	public:
		IRCServer( int, const std::string& );
		~IRCServer();

		void	start();
};

#endif