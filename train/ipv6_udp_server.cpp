//
//  ipv6_udp_server.cpp
//  webserv
//
//  Created by WildBoarGonnaGo on 26.06.2023.
//

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <cerrno>

void ErrOutput(const std::string &prefix) {
	std::cerr << prefix << strerror(errno) << std::endl;
	exit errno;
}

std::string NetToHostIPv6(const struct in6_addr &addr) {
	char addr6[INET6_ADDRSTRLEN];

	if (!inet_ntop(AF_INET6, &addr, addr6, INET6_ADDRSTRLEN))
		ErrOutput("error: can't cast address to string: ");
	return std::string(addr6);
}

int main(void) {
	int sock;
	uint16_t port = 49210;
	socklen_t len = sizeof(struct sockaddr_in6);
	struct sockaddr_in6 sv, cl;
	char buf[24];

	sock = socket(AF_INET6, SOCK_DGRAM, 0);
	if (sock == -1)
		ErrOutput("error: can't create socket: ");

	memset(&sv, 0, sizeof(struct sockaddr_in6));
	sv.sin6_addr = in6addr_any;
	sv.sin6_family = AF_INET6;
	sv.sin6_port = htons(port);

	if (bind(sock, (struct sockaddr *)&sv, sizeof(struct sockaddr_in6)) == -1)
		ErrOutput("error: can't bind socket to address (" + NetToHostIPv6(sv.sin6_addr) +
				  ", " + std::to_string(port) + "): ");
	while (true) {
		ssize_t num = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&cl, &len);
		if (num == -1)
			ErrOutput("error: can't read data from (" + NetToHostIPv6(cl.sin6_addr) +
					  ", " + std::to_string(ntohs(cl.sin6_port)) + "): ");
		else if (!num)
			std::cout << "No data is received from (" << NetToHostIPv6(cl.sin6_addr) <<
			", " << ntohs(cl.sin6_port) << ")" << std::endl;
		else {
			std::cout << num << " bytes of data were received from (" << NetToHostIPv6(cl.sin6_addr)
				<< ", " << ntohs(cl.sin6_port) << ")" << std::endl;
			for (int i = 0; i < num; ++i)
				buf[i] = toupper(buf[i]);
			if (sendto(sock, buf, num, 0, (struct sockaddr *)&cl, len) == -1)
				ErrOutput("error: can't send data to (" + NetToHostIPv6(sv.sin6_addr) +
						  ", " + std::to_string(ntohs(cl.sin6_port)) + "): ");
		}
	}
	if (close(sock) == -1)
		ErrOutput("error: can't close socket");
	return 0;
}
