//
//  ipv6_udp_client.cpp
//  client
//
//  Created by WildBoarGonnaGo on 27.06.2023.
//

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

int main(int argc, char *argv[]) {
	if (argc < 3 || !std::strcmp(argv[1], "--help")) {
		std::cout << "Usage: client <IPv6 address> <str_1> <str_2> ..." << std::endl;
		return 0;
	}

	int sock;
	uint16_t port = 49210;
	socklen_t len = sizeof(struct sockaddr_in6);
	struct sockaddr_in6 sv;
	char buf[512];

	sock = socket(AF_INET6, SOCK_DGRAM, 0);
	if (sock == -1)
		ErrOutput("error: can't create socket: ");
	memset(&sv, 0, sizeof(struct sockaddr_in6));
	if (inet_pton(AF_INET6, argv[1], &sv.sin6_addr) == -1)
		ErrOutput("error: can't assign address " + std::string(argv[1]) + ": ");
	sv.sin6_port = htons(port);
	sv.sin6_family = AF_INET6;

	for (int i = 2; i < argc; ++i) {
		ssize_t num = sendto(sock, argv[i], std::strlen(argv[i]), 0, (struct sockaddr *)&sv, len);
		if (num == -1)
			ErrOutput("error: can't send data to address (" + std::string(argv[1]) + ", " +
					  std::to_string(port) + "): ");
		else if (!num) {
			std::cout << "no data was sent to address (" << argv[1] << ", "
				<< port << ")" << std::endl;
			continue ;
		}
		else {
			std::memset(buf, 0, sizeof(buf));
			num = recvfrom(sock, buf, num, 0, (struct sockaddr *)&sv, &len);
			if (num == -1)
				ErrOutput("error: can't receive data from address (" + std::string(argv[1]) + ", " +
						  std::to_string(port) + "): ");
			else if (!num)
				std::cout << "Response " << i - 1 << ": no data is received" << std::endl;
			else
				std::cout << "Response " << i - 1 << ": " << buf << std::endl;
		}
	}
	if (close(sock) == -1)
		ErrOutput("error: can't close socket: ");
	return 0;
}
