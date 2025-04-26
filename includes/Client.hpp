/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:55:20 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/25 23:12:17 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "IRCServer.hpp"

class Channel;

class Client
{
	private:
		int						socket;
		std::string				nickname;
		std::string				username;
		std::string 			realname;
		bool					authenticated;
		bool					registered;
		bool					operators;
		std::vector<Channel*>	channels;

		Client();
		
	public:
		Client( int );
		Client( const Client& );
		Client& operator=( const Client& );
		~Client();

		int								getSocket();
		const std::string&				getNickname() const;
		const std::string&				getUsername() const;
		const std::string&				getRealname() const;
		const std::vector<Channel*>&	getChannels() const;

		bool							isAuthenticated();
		bool							isRegistered();
		bool							isOperators();
		
		void							setNickname( const std::string& );
		void							setUsername( const std::string& );
		void							setRealname( const std::string& );
		void							setAuthenticated();
		void							setRegistered();
		void							setOperators();
		void							addChannel( Channel* );
		void							removeChannel( Channel* );
		void							removeChannel( const std::string& );
};

#endif