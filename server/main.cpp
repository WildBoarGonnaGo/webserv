#include "headserv.hpp"
#include <unistd.h>
#include "Server.hpp"
#include "Client.hpp"

# define BIND_ERROR			"Error: bind: "
# define INET_ADDR_ERROR	"Error: inet_addr: "

#define SOCKPORT	8000
#define SOCKIP		"127.0.0.1"

# define LEN_QUEUE	128

# define PRINT(x) std::cout << x

void 		error(const char *err, const char *strerr)
{
	if (err)
		PRINT(err);
	if (strerr)
		PRINT(strerr);
	PRINT("\n");
}

void			sockcreate( Server &server )
{
	int 					res;
	int 					opt = 1;
	int 					pid;
	int 					cfd;

	server.setFdSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(sockport);
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_addr.s_addr = inet_addr(sockip);
	if (addr.sin_addr.s_addr == -1)
		error(INET_ADDR_ERROR, NULL);

	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	res = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	if (res == -1)
		error(BIND_ERROR, "Permission denied");
	listen(sockfd, LEN_QUEUE);
//	for(; ;)
//	{
//		slen = sizeof(addr);
//		cfd = accept(sockfd, (struct sockaddr*)&addr, &slen);
//		pid = fork();
//		if (!pid)
//		{
//			close(sockfd);
//			exit(0);
//		}
//		close(cfd);
//	}
}

int 		set_socket_time(Server &server)
{
	struct timeval	socktime;

	socktime.tv_sec = 3;
	socktime.tv_usec = 500000;
	setsockopt(server.getFdSocket(), SOL_SOCKET, SO_RCVTIMEO, &socktime, sizeof(socktime));
}

int 		socknonblock(int sockfd)
{
	int 	flags;

	flags = fcntl(sockfd, F_GETFL);
	fcntl(sockfd, F_SETFD, flags | O_NONBLOCK);
	return (0);
}

int			work_with_select(Server &server, Client &client)
{
	int 	fd;
	int 	res;
	fd_set	readfds;
	fd_set	writefds;
	int 	max_d;

	for(; ;)
	{
		max_d = sockfd;
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_SET(sockfd, &readfds);

	}
}

int			main()
{
	Server	server;

	server.setFdSocket(sockcreate(SOCKPORT, SOCKIP));
	set_socket_time(server);
	socknonblock(server);
	return (0);
}
