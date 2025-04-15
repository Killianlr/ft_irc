/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:43:49 by robincanava       #+#    #+#             */
/*   Updated: 2025/04/15 12:07:33 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "IRCException.hpp"
#include "Channel.hpp"

IRCServer::IRCServer( int port, const std::string& password ) : port(port), password(password), server_fd(-1) 
{
	channels["#general"] = new Channel("#general");
}

IRCServer::~IRCServer() {}

void    IRCServer::start()
{
	setupServer();
	runEventLoop();
}

void    IRCServer::setupServer()
{
	int                 opt = 1;
	struct sockaddr_in  address;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
}

void    IRCServer::runEventLoop()
{
	t_pollfd	server_poll_fd = {server_fd, POLLIN, 0};
	int			poll_count;

	poll_fds.push_back(server_poll_fd);
	while (true)
	{
		poll_count = poll(&poll_fds[0], poll_fds.size(), POLL_TIMEOUT);
		if (poll_count == -1)
		{
			perror("poll");
			exit(EXIT_FAILURE);
		}
		if (poll_fds[0].revents & POLLIN) //Tests whether new data or a new connection is available on the server socket(poll_fds[0]) by performing bitwise AND
			handleNewConnection();
		for (size_t i = 1; i < poll_fds.size(); i++)
			if (poll_fds[i].revents & POLLIN)
				handleClientData(poll_fds[i].fd);
	}
}

void	IRCServer::handleNewConnection()
{
	struct sockaddr_in	address;
	int					addrlen = sizeof(address);
	int					new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	
	if (new_socket == -1)
	{
		perror("accept");
		return ;
	}
	t_pollfd	new_poll_fd = {new_socket, POLLIN, 0};
	poll_fds.push_back(new_poll_fd);

	clients[new_socket] = new Client(new_socket);
	std::cout << "New client connected: " << new_socket << std::endl;
	channels["#general"]->addClient(clients[new_socket]); // ajout du nouveau client dans le channel #general
	
}

void	IRCServer::handleClientData( int client_socket )
{
	char			buffer[1024];
	int				valread = read(client_socket, buffer, 1024);
	CommandHandler	handler(this);

	if (valread <= 0)
	{
		closeClientConnection(client_socket);
		return ;
	}
	clientBuffers[client_socket].append(buffer, valread);

	size_t	pos;
	while ((pos = clientBuffers[client_socket].find("\r\n")) != std::string::npos)
	{
		std::string	message = clientBuffers[client_socket].substr(0, pos);
		clientBuffers[client_socket].erase(0, pos + 2);
		std::cout << "Received from client " << client_socket << ": " << message << std::endl;
		handler.handleCommand(client_socket, message);
	}
}

Client*	IRCServer::getClient( int socket ) const
{
	std::map<int, Client*>::const_iterator it = clients.find(socket);
	if (it != clients.end())
		return (it->second);
	return (0);
}

Client*	IRCServer::getClientByNickname( const std::string& nickname ) const
{
	for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return (it->second);
	}
	return (0);
}

std::vector<const Client*>	IRCServer::getListClients() const
{
	std::vector<const Client*>	list;
	for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); it++)
		list.push_back(it->second);
	return (list);
}

const std::string&	IRCServer::getPassword() const
{
	return (this->password);
}

Channel* IRCServer::getChannel( std::string channel_name )
{
	std::map<std::string, Channel*>::iterator it = channels.find(channel_name);
	if (it != channels.end())
		return (it->second);
	return (0);
}

void	IRCServer::addChannel(std::string channel_name, Channel* newChannel)
{
	channels[channel_name] = newChannel;
}

void	IRCServer::closeClientConnection( int client_socket )
{
	close(client_socket);
	delete clients[client_socket];
	clientBuffers.erase(client_socket);
	clients.erase(client_socket);
	for (std::vector<t_pollfd>::iterator it = poll_fds.begin(); it != poll_fds.end(); it++)
	{
		if (it->fd == client_socket)
		{
			poll_fds.erase(it);
			break ;
		}
	}
	std::cout << "Client disconnected: " << client_socket << std::endl;
	return ;
}

bool	IRCServer::isClientInChannel( int client_socket, const std::string& channel_name ) const
{
	std::map<std::string, Channel*>::const_iterator it = channels.find(channel_name);

	if (it == channels.end())
		return (false);
	
	Channel*	channel = it->second;
	Client*		client = getClient(client_socket);

	if (!client)
		return (false);
	return (channel->hasClient(client));
}

std::vector<Client*>	IRCServer::getClientsInChannel( const std::string& channel_name ) const
{
	std::vector<Client*>	empty;
	std::map<std::string, Channel*>::const_iterator it = channels.find(channel_name);

	if (it == channels.end())
		return (empty);
	return (it->second->getMembers());
}