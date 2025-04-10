/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard42 <rrichard42@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:55:20 by rrichard42        #+#    #+#             */
/*   Updated: 2025/04/05 15:16:18 by rrichard42       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "IRCServer.hpp"

class Channel;

class Client
{
	private:
		int			socket;
		std::string	nickname;
		std::string	username;
		std::string realname;
		bool		authenticated;
		bool		registered;
		bool		operators;

		Channel*	currentChannel;

		Client();
		
	public:
		Client( int );
		Client( const Client& );
		Client& operator=( const Client& );
		~Client();

		int					getSocket();
		const std::string&  getNickname() const;
		const std::string&	getUsername() const;
		const std::string&	getRealname() const;
		bool				isAuthenticated();
		bool				isRegistered();
		bool				isOperators();
		Channel*			getCurrentChannel() const;
		
		void				setNickname( const std::string& );
		void				setUsername( const std::string& );
		void				setRealname( const std::string& );
		void				setAuthenticated();
		void				setRegistered();
		void				setOperators();
		void				setCurrentChannel(Channel* Channel);
};

#endif