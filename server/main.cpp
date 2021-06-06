#include "headserv.hpp"

# define BIND_ERROR			"Error: bind: "
# define INET_ADDR_ERROR	"Error: inet_addr: "

#define SOCKPORT	8000
#define SOCKIP		"127.0.0.1"

# define LEN_QUEUE	128

# define PRINT(x) std::cout << x << std::endl

void 		error(const char *err, const char *strerr)
{
	PRINT(err << strerr);
}

int			sockcreate(int sockport, const char *sockip)
{
	struct sockaddr_in		addr;
	int						sockfd;
	int 					res;
	int 					opt = 1;
	int 					pid;
	socklen_t				slen;
	int 					cfd;

	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(sockport);
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_addr.s_addr = inet_addr(sockip);
	if (addr.sin_addr.s_addr == -1)
		error(INET_ADDR_ERROR, "");

	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	res = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	if (res == -1)
		error(BIND_ERROR, "Permission denied");
	listen(sockfd, LEN_QUEUE);
	for(; ;)
	{
		slen = sizeof(addr);
		cfd = accept(sockfd, (struct sockaddr*)&addr, &slen);
		pid = fork();
		if (!pid)
		{
			close(sockfd);
			exit(0);
		}
		close(cfd);

	}
}

int 		set_socket_time(int sockfd)
{
	struct timeval	socktime;

	socktime.tv_sec = 3;
	socktime.tv_usec = 500000;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &socktime, sizeof(socktime));
}

int 		socknonblock(int sockfd)
{
	int 	flags;

	flags = fcntl(sockfd, F_GETFL);
	fcntl(sockfd, F_SETFD, flags | O_NONBLOCK);
}

int			main()
{
	sockcreate(SOCKPORT, SOCKIP);
	set_socket_time();
	return (0);
}
