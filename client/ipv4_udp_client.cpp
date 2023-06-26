#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <string>
#include <netinet/in.h>
#include <cerrno>
#include <iostream>

void ErrOutput(const std::string &prefix) {
	std::cerr << prefix << strerror(errno) << std::endl;
	exit errno;
}

/*std::string HostByteOrderPres(const in_addr_t &src) {
	char addr[INET_ADDRSTRLEN];

	uint32_t aux = ntohl(src);
	inet_ntop(AF_INET, &aux, addr, INET_ADDRSTRLEN);
	return std::string(addr);
}*/

int main(int argc, char *argv[]) {
	int clSock;
	uint16_t port = 49200;
	sockaddr_in sv;
	socklen_t len = sizeof(struct sockaddr_in);
	char buf[512];

	std::memset(buf, 0, sizeof(buf));
	if (argc < 3 || !std::strcmp(argv[1], "--help")) {
		std::cout << "Usage: client <ip address> <str_1> <str_2> ..." << std::endl;
		return 0;
	}
	clSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (clSock == -1)
		ErrOutput("error: can't create socket: ");

	std::memset(&sv, 0, sizeof(sv));
	sv.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &sv.sin_addr.s_addr);
	sv.sin_port = htons(port);

	for (int i = 2; i < argc; ++i) {
		ssize_t bytes;
		std::memset(buf, 0, 512);
		if (sendto(clSock, argv[i], std::strlen(argv[i]), 0, (struct sockaddr *)&sv, len) == -1)
			ErrOutput("error: can't send data to (" + std::string(argv[1])
					  + ", " + std::to_string(port) + "): ");
		bytes = recvfrom(clSock, buf, sizeof(buf), 0, nullptr, nullptr);
		if (bytes == -1)
			ErrOutput("error: can't recieve data from server (" + std::string(argv[1])
					  + ", " + std::to_string(port) + ")");
		else if (!bytes)
			std::cout << "Response " << i - 1 << ": no data is recieved" << std::endl;
		else
			std::cout << "Response " << i - 1 << ": " << buf << std::endl;
	}
	if (close(clSock) == -1)
		ErrOutput("error: can't close socket");
	return 0;
}
