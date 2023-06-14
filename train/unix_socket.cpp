#include <sys/socket.h>
#include <sys/un.h>
#include <string>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <unistd.h>

#define BACKLOG 5

void ErrOutput(std::string errPrefix) {
	std::cerr << errPrefix << strerror(errno) << std::endl;
	exit (errno);
}

int main(void) {
	const std::string path("/tmp/web.sock");
	int sfd, cfd;
	struct sockaddr_un unS;

	if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
		ErrOutput("error: can't create socket: ");
	if (remove(path.c_str()) == -1 && errno != ENOENT)
		ErrOutput("error: socket file: ");
	std::memset(&unS, 0, sizeof(unS));
	unS.sun_family = AF_UNIX;
	std::strncpy(unS.sun_path, path.c_str(), sizeof(unS.sun_path) - 1);
	if ((bind(sfd, (struct sockaddr *)&unS, sizeof(struct sockaddr_un))) == -1)
		ErrOutput("error: can't bind socket to addr: " + std::string(unS.sun_path) + ": ");
	if ((listen(sfd, BACKLOG)) == -1)
		ErrOutput("error: server can't listen to peers: ");
	while (true) {
		int b;
		char buf[1024] = { 0 };
		if ((cfd = accept(sfd, nullptr, nullptr)) == -1)
			ErrOutput("error: can't create reliable connection between peer and server");
		while ((b = read(cfd, &buf, 1024)) > 0) {
			if ((write(STDOUT_FILENO, &buf, b)) != b)
				ErrOutput("error: partial/failed write: ");
			std::memset(&buf, 0, 1024);
		}
		if (b == -1)
			ErrOutput("error: read client message: ");
		if (close(cfd) == -1)
			ErrOutput("error: can't close client socket");
	}
	if (close(sfd) == -1)
		ErrOutput("error: can't close server socket");
	return 0;
}
