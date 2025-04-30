/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:04:35 by robincanava       #+#    #+#             */
/*   Updated: 2025/04/30 18:12:41 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"

volatile sig_atomic_t   running = 1;

void signalHandler( int signum )
{
	(void)signum;
	std::cout << "\nSIGINT signal received\n";
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