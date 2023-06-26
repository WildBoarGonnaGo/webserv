//
//  ipv4_udp_server.cpp
//  webserv
//
//  Created by WildBoarGonnaGo on 25.06.2023.
//

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <cctype>

void ErrOutput(const std::string &prefix) {
	std::cerr << prefix << strerror(errno) << std::endl;
	exit errno;
}

std::string HostOrderAddrPres(const in_addr_t &src) {
	char aux[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &src, aux, INET_ADDRSTRLEN);
	return std::string(aux);
}

int main(void) {
	int svSock;
	uint16_t port = 49200;
	struct sockaddr_in sv, cl;
	socklen_t len;
	char buf[24] = { 0 };
	char addr[INET_ADDRSTRLEN] = { 0 };

	svSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (svSock == -1)
		ErrOutput("error: can't create socket: ");

	std::memset(&sv, 0, sizeof(struct sockaddr_in));
	sv.sin_family = AF_INET;
	sv.sin_port = htons(port);
	sv.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(svSock, (struct sockaddr *)&sv, sizeof(struct sockaddr_in)) == -1)
		ErrOutput("error: can't bind to address (" + HostOrderAddrPres(sv.sin_addr.s_addr) + ", " +
				  std::to_string(port) + "): ");

	while(true) {
		socklen_t len = sizeof(struct sockaddr_in);
		ssize_t bytes = recvfrom(svSock, buf, 24, 0, (struct sockaddr *)&cl, &len);
		if (bytes == -1)
			ErrOutput("error: can't read from (" + HostOrderAddrPres(cl.sin_addr.s_addr) + ", "
					  + std::to_string(ntohs(cl.sin_port)) + "): ");
		if (bytes == 0)
			continue ;
		std::cout << "Server recieved " << bytes << " from (" + HostOrderAddrPres(cl.sin_addr.s_addr)
			+ ", " + std::to_string(ntohs(cl.sin_port)) << ")" << std::endl;
		for (ssize_t i = 0; i < bytes; ++i)
			buf[i] = toupper(buf[i]);
		if (sendto(svSock, buf, bytes, 0, (struct sockaddr *)&cl, len) == -1)
			ErrOutput("error: can't send to (" + HostOrderAddrPres(cl.sin_addr.s_addr) + ", "
					  + std::to_string(ntohs(cl.sin_port)) + ": ");
		memset(buf, 0, sizeof(buf));
	}
	if (close(svSock) == -1)
		ErrOutput("error: can't close socket: ");
	return 0;
}
