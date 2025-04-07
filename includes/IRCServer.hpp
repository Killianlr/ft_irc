/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:44:15 by robincanava       #+#    #+#             */
/*   Updated: 2025/04/05 15:36:51 by rrichard42       ###   ########.fr       */
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
# include "CommandHandler.hpp"
# include "Client.hpp"

typedef struct pollfd t_pollfd;

const int	POLL_TIMEOUT = 1000;

class Client;
class Channel;

class IRCServer
{
	private:
		int						port;
		std::string				password;
		int						server_fd;
		std::vector<t_pollfd>	poll_fds;
		std::map<int, Client*>	clients;
		std::map<std::string, Channel*> channels;

		IRCServer&	operator=( const IRCServer& );
		IRCServer( IRCServer& );

		void	setupServer();
		void	runEventLoop();
		void	handleNewConnection();
		void	handleClientData( int );
	
	public:
		IRCServer( int, const std::string& );
		~IRCServer();

		void							start();
		Client*							getClient( int );
		std::vector<const Client*>		getListClients() const;
		const std::string&				getPassword() const;
		Channel*						getChannel( std::string );

		void							addChannel(std::string channel_name, Channel* newChannel);
};

#endif