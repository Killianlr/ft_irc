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
		std::vector<Client*>	_operators;
		std::string				_topic;
		
		bool					_inviteOnly;
		bool					_topicRestricted;
		std::string				_key;
		int						_userLimit;

		Channel();
		Channel(Channel&);
		
	public:
		Channel(const std::string& name);
		Channel &operator=( const Channel& );
		~Channel();

		
		const std::string				&getKey() const;
		const std::string 				&getName() const;
		const std::vector<Client *>		&getMembers() const;
		int								getUserLimit() const;
		int								getNbMembers() const;
		std::vector<Client*>			getClients() const;
		const std::string				&getTopic() const;

		void	addClient(Client* client);
		void	addInvite(Client* client);
		void	removeClient(Client* client);

		void	setOperator(Client* client);
		void	setInviteOnly(bool value);
		void	setTopicRestricted(bool value);
		void	setKey(const std::string k);
		void	setUserLimit(int limit);
		void	setTopic( const std::string& );
		
		bool	isInvite(Client* client) const;
		bool	isOperator(Client* client) const;
		bool	isInviteOnly() const;
		bool	isTopicRestricted() const;
		bool	hasClient( const Client* ) const;

};

#endif