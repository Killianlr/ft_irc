/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: robincanavaggio <robincanavaggio@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:04:35 by robincanava       #+#    #+#             */
/*   Updated: 2025/04/02 19:50:10 by robincanava      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCServer.hpp"

int main( int argc, char *argv[] )
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return (1);
    }
    
    int         port = std::stoi(argv[1]);
    std::string password = argv[2];
    IRCServer   server(port, password);
    server.start();
    return (0);
}