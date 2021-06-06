#ifndef SERVER_HPP
# define SERVER_HPP


class		Server
{
public:
	Server();
	~Server();
	Server( const Server &s );
	Server				operator=( const Server &s );
	void				setFdSocket();
	int					getFdSocket() const;
private:
	int			fdSocket;
};

#endif
