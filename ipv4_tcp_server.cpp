//
//  ipv4_tcp_server.cpp
//  webserv
//
//  Created by WildBoarGonnaGo on 27.06.2023.
//

#include "readline.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <cstdlib>
#include <climits>

int main(int argc, char *argv[]) {
	int seqNum, sock, numLen = std::to_string(INT_MAX).size() + 1;
	uint16_t port;
	char seqNumStr[numLen];
	char reqNumStr[numLen];
	char host[NI_MAXHOST];
	char srv[NI_MAXSERV];
	struct addrinfo hints;
	struct addrinfo *candi = nullptr, *roll = nullptr;

	std::memset(seqNumStr, 0, numLen);
	std::strcpy(seqNumStr, (argc > 1) ? argv[1] : "1");
	std::memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_NUMERICSERV | AI_ADDRCONFIG | AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int geta = getaddrinfo(nullptr, srv, &hints, &candi);

	if (geta) {
		std::cerr << "error: can't get address sequence: "
			<< gai_strerror(geta) << std::endl;
		if (candi)
			freeaddrinfo(candi);
		return -1;
	}

	for (roll = candi; roll; roll = roll->ai_next) {
		int sockopt;

		sock = socket(roll->ai_family, roll->ai_socktype, roll->ai_socktype);
		if (sock == -1)
			continue ;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) == -1)
			SysError("error: can't set option for socket: ");
		if (bind(sock, (struct sockaddr *)roll->ai_addr, roll->ai_addrlen) == -1)
			SysEror("error: can't bind socket to address (");
	}
}
