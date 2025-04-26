/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:44:15 by robincanava       #+#    #+#             */
/*   Updated: 2025/04/22 12:07:51 by rrichard         ###   ########.fr       */
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
# include <algorithm>
# include <csignal>
# include <ctime>
# include "CommandHandler.hpp"
# include "Client.hpp"
# include "Channel.hpp"

#define ROUGE "\033[0;31m"
#define VERT "\033[0;32m"
#define BLEU "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define RESET   "\033[0m"


typedef struct pollfd t_pollfd;

const int	POLL_TIMEOUT = 1000;

class Client;
class Channel;

class IRCServer
{
	private:
		int								port;
		std::string						password;
		int								server_fd;
		std::vector<t_pollfd>			poll_fds;
		std::map<int, Client*>			clients;
		std::map<std::string, Channel*> channels;

		IRCServer&	operator=( const IRCServer& );
		IRCServer( IRCServer& );

		void	setupServer();
		void	runEventLoop();
		void	handleNewConnection();
		void	handleClientData( int );
		void	checkCurrentChannels();
	
	public:
		std::map<int, std::string>	clientBuffers;

		IRCServer( int, const std::string& );
		~IRCServer();

		Client*									getClient( int ) const;
		Client*									getClientByNickname( const std::string& ) const;
		std::vector<const Client*>				getListClients() const;
		const std::string&						getPassword() const;
		Channel*								getChannel( std::string );
		
		void									start();
		void									closeClientConnection( int );
		void									addChannel(std::string channel_name, Channel* newChannel);
		bool									isClientInChannel( int, const std::string& ) const;
		std::vector<Client*>					getClientsInChannel( const std::string& ) const;
		const std::map<std::string, Channel*>&	getChannels() const;
};

#endif