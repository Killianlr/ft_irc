/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:43:49 by robincanava       #+#    #+#             */
/*   Updated: 2025/04/03 20:31:16 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCServer.hpp"

IRCServer::IRCServer( int port, const std::string& password ) : port(port), password(password), server_fd(-1)
{
	commands["NICK"] = &IRCServer::cmdNick;
	commands["USER"] = &IRCServer::cmdUser;
	commands["JOIN"] = &IRCServer::cmdJoin;
	commands["PRIVMSG"] = &IRCServer::cmdPrivmsg;
	commands["PING"] = &IRCServer::cmdPing;
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

	Client	newClient = { new_socket, "", "", false };
	clients[new_socket] = newClient;
	std::cout << "New client connected: " << new_socket << std::endl;
}

void	IRCServer::handleClientData( int client_socket )
{
	char	buffer[1024];
	int		valread = read(client_socket, buffer, 1024);
	if (valread <= 0)
	{
		close(client_socket);
		clients.erase(client_socket);
		// Remove from poll_fds as well
		std::cout << "Client disconnected: " << client_socket << std::endl;
		return ;
	}
	std::string	message(buffer);
	std::cout << "Received from client " << client_socket << ": " << message << std::endl;
	// IRC COMMANDS HERE
	handleCommand(client_socket, message);
}

void	IRCServer::handleCommand( int client_socket, const std::string& message )
{
	size_t		pos = message.find(' ');
	std::string	command = message.substr();
	std::string	param;
	if (pos != std::string::npos)
		param = message.substr(pos + 1);
	else
		param = "";
	if (commands.find(command) != commands.end())
		(this->*commands[command])(client_socket, param);
}