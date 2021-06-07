#include "Server.hpp"

Server::Server() : _sockfd(0), _sockip("0.0.0.0"), _socklen(0)
{
}

Server::~Server()
{
}

Server::Server( Server const &s )
{
	*this = s;
}

Server&				Server::operator=( Server const &s )
{
	if (this != &s)
	{
		;
	}
	return *this;
}

void 				Server::setFdSocket( int sockfd )
{
	_sockfd = sockfd;
}

int 				Server::getFdSocket() const
{
	return _sockfd;
}

void				Server::setIPSocket( const char *sockip )
{
	_sockip = sockip;
}
const char*			Server::getIPSocket() const
{
	return _sockip;
}