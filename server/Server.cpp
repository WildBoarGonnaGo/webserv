#include "Server.hpp"

webserv::Server::Server()
//: _domain(AF_INET), _service(SOCK_STREAM), _protocol(IPPROTO_TCP)
{
//	_sockaddr.sin_family = _domain;
//	_sockaddr.sin_port = htons(8080);
//	_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
}

webserv::Server::Server( int domain, int port, const char *interface )
: _domain(domain), _service(SOCK_STREAM), _protocol(IPPROTO_TCP)
{
	if (createNewSocket() < 0)
		exit(EXIT_FAILURE);
	if (initSockaddrStruct(port, interface) == -1)
		exit(EXIT_FAILURE);
	if (sockBind() < 0)
		exit(EXIT_FAILURE);
}

webserv::Server::Server( int domain, int service, int port, const char *interface )
: _domain(domain), _service(service), _protocol(IPPROTO_TCP)
{
	if (createNewSocket() < 0)
		exit(EXIT_FAILURE);
	if (initSockaddrStruct(port, interface) == -1)
		exit(EXIT_FAILURE);
	if (sockBind() < 0)
		exit(EXIT_FAILURE);
}

webserv::Server::Server( int domain, int service, int protocol, int port, const char *interface )
: _domain(domain), _service(service), _protocol(protocol)
{
	if (createNewSocket() < 0)
		exit(EXIT_FAILURE);
	if (initSockaddrStruct(port, interface) == -1)
		exit(EXIT_FAILURE);
	if (sockBind() < 0)
		exit(EXIT_FAILURE);
}

webserv::Server::~Server()
{
}

webserv::Server::Server( Server const &s )
{
	*this = s;
}

Server&						webserv::Server::operator=( Server const &s )
{
	if (this != &s)
	{
		;
	}
	return *this;
}

void 						webserv::Server::setFdSocket( int sockfd )
{
	_sockfd = sockfd;
}

int 						webserv::Server::getFdSocket() const
{
	return _sockfd;
}

const char*					webserv::Server::getIPSocket() const
{
	return _sockip;
}

int 						webserv::Server::initSockaddrStruct( int port, const char *interface )
{
//	memset((char *)&_sockaddr, 0, sizeof(_sockaddr));
	_sockaddr.sin_family = _domain;
	_sockaddr.sin_port = htons(port);
	_sockaddr.sin_addr.s_addr = inet_addr(interface);
	if (_sockaddr.sin_addr.s_addr == -1)
		return (-1);
	memset(_sockaddr.sin_zero, 0, sizeof(_sockaddr.sin_zero));
	return (0);
}

int 						webserv::Server::createNewSocket()
{
	_sockfd = socket(_domain, _service, _protocol);
	if (_sockfd < 0)
		return (-1);
	return (0);
}

int 						webserv::Server::sockBind()
{
	int 	res;

	res = bind(_sockfd, (const sockaddr*)&_sockaddr, sizeof(_sockaddr));
	if (res < 0)
		return (-1);
	return (0);
}

int 						webserv::Server::getDomain() const
{
	return _domain;
}

int 						webserv::Server::getService() const
{
	return _service;
}

int 						webserv::Server::getProtocol() const
{
	return _protocol;
}

int							webserv::Server::getSockFd() const
{
	return _sockfd;
}

const struct sockaddr_in	&webserv::Server::getSockAddr() const
{
	return _sockaddr;
}
