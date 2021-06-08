#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

namespace		webserv
{
	class		Server
	{
	public:
		Server( int domain, int port, const char *interface );
		Server( int domain, int service, int port, const char *interface );
		Server( int domain, int service, int protocol, int port, const char *interface );
		~Server();
		Server(const Server &s);
		Server						&operator=(const Server &s);
		int 						getDomain() const;
		int 						getService() const;
		int 						getProtocol() const;
		int							getSockFd() const;
		const struct sockaddr_in	&getSockAddr() const;

	private:
		int 				_domain;
		int 				_service;
		int 				_protocol;

		int					_sockfd;

		struct sockaddr_in	_sockaddr;

		Server();
		int 				initSockaddrStruct( int port, const char *interface );
		int 				createNewSocket();
		int 				sockBind();
	};
}


#endif
