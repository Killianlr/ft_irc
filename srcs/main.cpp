/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:04:35 by robincanava       #+#    #+#             */
/*   Updated: 2025/04/16 13:23:54 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"

volatile sig_atomic_t   running = 1;

void signalHandler( int signum )
{
	std::string	sigstring;

	switch (signum)
	{
		case SIGINT:
			sigstring = "SIGINT";
			break;
		case SIGTERM:
			sigstring = "SIGTERM";
			break;
		case SIGQUIT:
			sigstring = "SIGQUIT";
			break;
		default:
			sigstring = "Unknown signal";
			break;
	}
	std::cout << "Signal received: " << sigstring << std::endl;
	running = 0;
}

int main( int argc, char *argv[] )
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return (1);
	}

	std::signal(SIGINT, signalHandler);
	std::istringstream  iss(argv[1]);
	int port;

	if (!(iss >> port))
	{
		std::cerr << "Invalid port number: " << argv[1] << std::endl;
		return (1);
	}
	std::string password = argv[2];
	IRCServer   server(port, password);
	server.start();
	return (0);
}