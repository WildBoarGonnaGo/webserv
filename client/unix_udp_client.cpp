#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cerrno>

void ErrOutput(std::string prefix) {
	std::cerr << prefix << strerror(errno) << std::endl;
	exit (errno);
}

int main(int argc, char *argv[]) {
	if (argc < 2 || !strcmp(argv[1], "--help")) {
		std::cout << "Usage: client <str1> <str2> ..." << std::endl;
		return 0;
	}

	struct sockaddr_un sv, cl;
	// socklen_t len = sizeof(struct sockaddr_un);
	std::string sv_path = "/tmp/sv_udp.soc";
	std::string cl_path = "/tmp/cl_udp." + std::to_string(getpid()) + ".soc";
	char buf[128] = { 0 };

	int clSock = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (clSock == -1)
		ErrOutput("error: can't create socket: ");

	std::memset(&cl, 0, sizeof(cl));
	cl.sun_family = AF_UNIX;
	std::strncpy(cl.sun_path, cl_path.c_str(), cl_path.size());

	if (bind(clSock, (struct sockaddr *)&cl, sizeof(struct sockaddr_un)) == -1)
		ErrOutput("error: can't bind to address: " + std::string(cl.sun_path) + ": ");

	std::memset(&sv, 0, sizeof(sv));
	sv.sun_family = AF_UNIX;
	std::strncpy(sv.sun_path, sv_path.c_str(), sv_path.size());

	for (int i = 1; i < argc; ++i) {
		if (sendto(clSock, argv[i], std::strlen(argv[i]), 0, (struct sockaddr *)&sv, sizeof(struct sockaddr_un)) == -1)
			ErrOutput("error: can't send info to " + std::string(sv.sun_path) + " from: " + std::string(cl.sun_path) + ": ");
		ssize_t numBuffer = recvfrom(clSock, buf, 128, 0, nullptr, nullptr);
		if (numBuffer == -1)
			ErrOutput("error: can't get server reply from: " + std::string(sv.sun_path) + ": ");
		std::cout << "Recieved " << numBuffer << " bytes from server: " << buf << std::endl;
		std::memset(buf, 0, sizeof(buf));
	}
	if (close(clSock) == -1)
		ErrOutput("error: can't close socket: ");
	return 0;
}
