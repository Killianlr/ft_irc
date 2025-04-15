#include "Channel.hpp"

Channel::Channel(const std::string &name) : _name(name)
{
	_inviteOnly = false;
	_topicRestricted = false;
	_key = "";
	_userLimit = -1;
}

Channel::~Channel() {}

void	Channel::addClient(Client* client)
{
	if (!hasClient(client))
	{
		_members.push_back(client);
		_operators[client->getSocket()] = client->isOperators();
		client->setCurrentChannel(this);
	}
}

void	Channel::addInvite(Client* client)
{
	if (!isInvite(client))
		_invited.push_back(client);
}

void	Channel::setOperator(Client* client, bool status)
{
	_operators[client->getSocket()] = status;
}

void	Channel::removeClient(Client* client)
{
	for (std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); ++it)
	{
		if (*it == client)
		{
			_members.erase(it);
			break;
		}
	}
	_operators.erase(client->getSocket());
}

bool	Channel::hasClient(Client* client) const
{
	for (size_t i = 0; i < _members.size(); ++i)
	{
		if (_members[i] == client)
		return true;
	}
	return false;
}

bool	Channel::isInvite(Client* client) const
{
	for (size_t i = 0; i < _invited.size(); ++i)
	{
		if (_invited[i] == client)
		return true;
	}
	return false;
}

bool	Channel::isOperator(Client* client) const
{
	std::map<int, bool>::const_iterator it = _operators.find(client->getSocket());
	if (it != _operators.end())
		return (it->second);
	return false;
}

const std::string &Channel::getName() const
{
	return _name;
}

const std::vector<Client *>	&Channel::getMembers() const
{
	return _members;
}

void	Channel::setInviteOnly(bool value)
{
	_inviteOnly = value;
}

void	Channel::setTopicRestricted(bool value)
{
	_topicRestricted = value;
}

void	Channel::setKey(const std::string k)
{
	_key = k;
}

void	Channel::setUserLimit(int limit)
{
	_userLimit = limit;
}

bool	Channel::isInviteOnly() const
{
	return _inviteOnly;
}

bool	Channel::isTopicRestricted() const
{
	return _topicRestricted;
}

int		Channel::getUserLimit() const
{
	return _userLimit;
}

const	std::string	&Channel::getKey() const
{
	return _key;
}

int		Channel::getNbMembers() const
{
	return _members.size();
}