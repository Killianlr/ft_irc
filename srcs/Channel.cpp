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