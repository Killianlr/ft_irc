#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "IRCServer.hpp"

class Channel
{
	private:
		std::string _name;
		std::vector<Client *> _members;

		Channel();
		Channel(Channel&);
		Channel &operator=(Channel &);

	public:
		Channel(const std::string& name);
		~Channel();

		void					addClient( Client* client );
		bool					hasClient( const Client* ) const;
		const std::string		&getName() const;
		std::vector<Client*>	getClients() const;

};

#endif