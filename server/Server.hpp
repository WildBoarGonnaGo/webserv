#ifndef SERVER_HPP
# define SERVER_HPP


class		Server
{
public:
	Server();
	~Server();
	Server( const Server &s );
	Server&				operator=( const Server &s );
	void				setFdSocket( int sockfd );
	int					getFdSocket() const;
	void				setIPSocket( const char *sockip );
	const char			*getIPSocket() const;

private:
	int					_sockfd;
	char				*_sockip;
	struct sockaddr_in	_sockaddr;
	socklen_t			_socklen;
};



#endif
