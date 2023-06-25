#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <cctype>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <string>
#include <cstdio>
#include <unistd.h>

#define BUF_SIZE 20

void ErrOutput(std::string prefix) {
	std::cerr << prefix << strerror(errno) << std::endl;
	exit (errno);
}

int main(void) {
	struct sockaddr_un sv, cl;
	int svSock;
	socklen_t len;
	std::string svPath = "/tmp/sv_udp.soc";
	char buf[BUF_SIZE] = { 0 };

	svSock = socket(AF_UNIX, SOCK_DGRAM | SOCK_CLOEXEC, 0);
	if (svSock == -1)
		ErrOutput("error: can't create socket: ");

	/*Bind socket to a well known address*/
	if (remove(svPath.c_str()) == -1 && errno != ENOENT)
		ErrOutput("error: file deletion: " + svPath + ": ");

	std::memset(&sv, 0, sizeof(struct sockaddr_un));
	sv.sun_family = AF_UNIX;
	std::strncpy(sv.sun_path, svPath.c_str(), svPath.size());

	if (bind(svSock, (struct sockaddr *)&sv, sizeof(struct sockaddr_un)) == -1)
		ErrOutput("error: can't bind socket address: " + svPath + ": ");
	while (true) {
		ssize_t numBuffer;
		len = sizeof(struct sockaddr_un);
		numBuffer = recvfrom(svSock, buf, BUF_SIZE, 0, (struct sockaddr *)&cl, &len);
		if (numBuffer == -1)
			ErrOutput("error: can't read datagram: ");

		for (ssize_t i = 0; i < numBuffer; ++i)
			buf[i] = std::toupper(buf[i]);
		std::cout << numBuffer << " bytes recieved from " << cl.sun_path << std::endl;
		if (sendto(svSock, buf, numBuffer, 0, (struct sockaddr *)&cl, len) == -1)
			ErrOutput("error: can't send info: " + std::string(cl.sun_path) + ": ");
		std::memset(buf, 0, BUF_SIZE);
	}
	if (close(svSock) == -1)
		ErrOutput("error: can't close socket");
	return 0;
}
