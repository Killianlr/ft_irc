#include "Channel.hpp"

Channel::Channel(const std::string &name) : _name(name) {}

Channel::~Channel() {}

void	Channel::addClient(Client* client)
{
	_members.push_back(client);
}

const std::string &Channel::getName() const
{
	return _name;
}

bool	Channel::hasClient( const Client* client ) const
{
	for (std::vector<Client*>::const_iterator it = _members.begin(); it != _members.end(); it++)
	{
		if (*it == client)
			return (true);
	}
	return (false);
}

std::vector<Client*>	Channel::getClients() const
{
	return (this->_members);
}