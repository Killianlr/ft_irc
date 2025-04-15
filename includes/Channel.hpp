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

		
		const std::string				&getKey() const;
		const std::string 				&getName() const;
		const std::vector<Client *>		&getMembers() const;
		int								getUserLimit() const;
		int								getNbMembers() const;
		std::vector<Client*>			getClients() const;

		void	addClient(Client* client);
		void	addInvite(Client* client);
		void	removeClient(Client* client);

		void	setOperator(Client* client, bool status);
		void	setInviteOnly(bool value);
		void	setTopicRestricted(bool value);
		void	setKey(const std::string k);
		void	setUserLimit(int limit);
		
		bool	isInvite(Client* client) const;
		bool	isOperator(Client* client) const;
		bool	isInviteOnly() const;
		bool	isTopicRestricted() const;
		bool	hasClient( const Client* ) const;

};

#endif