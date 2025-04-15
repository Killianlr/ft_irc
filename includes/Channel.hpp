#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "IRCServer.hpp"

class Client;

class Channel
{
	private:
		std::string 			_name;
		std::vector<Client *> 	_members;
		std::vector<Client *>	_invited;
		std::map<int, bool>		_operators;
		
		bool					_inviteOnly;
		bool					_topicRestricted;
		std::string				_key;
		int						_userLimit;

		Channel();
		Channel(Channel&);
		Channel &operator=(Channel &);

	public:
		Channel(const std::string& name);
		~Channel();

		void	addClient(Client* client);
		void	addInvite(Client* client);
		void	setOperator(Client* client, bool status);
		void	removeClient(Client* client);
		void	setInviteOnly(bool value);
		void	setTopicRestricted(bool value);
		void	setKey(const std::string k);
		void	setUserLimit(int limit);
		
		bool	hasClient(Client* client) const;
		bool	isInvite(Client* client) const;
		bool	isOperator(Client* client) const;
		bool	isInviteOnly() const;
		bool	isTopicRestricted() const;
		int		getUserLimit() const;
		int		getNbMembers() const;
		
		const std::string				&getKey() const;
		const std::string 				&getName() const;
		const std::vector<Client *>		&getMembers() const;

};

#endif