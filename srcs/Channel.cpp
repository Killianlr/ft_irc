#include "Channel.hpp"

Channel::Channel(const std::string &name) : _name(name)
{
	_inviteOnly = false;
	_topicRestricted = false;
	_key = "";
	_userLimit = -1;
	_topic = "";
}

Channel::~Channel() {}

Channel&	Channel::operator=( const Channel& other )
{
	if (this != &other)
	{
		this->_name = other._name;
		this->_members = other._members;
		this->_invited = other._invited;
		this->_operators = other._operators;
		this->_topic = other._topic;
		this->_inviteOnly = other._inviteOnly;
		this->_topicRestricted = other._topicRestricted;
		this->_key = other._key;
		this->_userLimit = other._userLimit;
	}
	return (*this);
}

void	Channel::addClient(Client* client)
{
	if (!hasClient(client))
	{
		_members.push_back(client);
		// _operators[client->getSocket()] = client->isOperators();
		client->setCurrentChannel(this);
	}
}

void	Channel::addInvite(Client* client)
{
	if (!isInvite(client))
		_invited.push_back(client);
}

void	Channel::setOperator(Client* client)
{
	_operators.push_back(client);
}

void	Channel::removeClient(Client* client)
{
	if (isOperator(client))
	{
		for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); it++)
		{
			if (*it == client)
			{
				_operators.erase(it);
				break ;
			}
		}
	}
	for (std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); ++it)
	{
		if (*it == client)
		{
			_members.erase(it);
			break;
		}
	}
}

bool	Channel::hasClient(const Client* client) const
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
	for (std::vector<Client*>::const_iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == client)
			return (true);
	}
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

const std::string&	Channel::getTopic() const
{
	return (this->_topic);
}

void	Channel::setTopic( const std::string& str )
{
	this->_topic = str;
}