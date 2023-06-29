//
//  ipv4_tcp_server.cpp
//  webserv
//
//  Created by WildBoarGonnaGo on 27.06.2023.
//

#include "../readline.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <cstdlib>
#include <climits>


/*void *InetAddrInfo(const struct addrinfo *info) {
	struct sockaddr_in *ipv4_addr = nullptr;
	struct sockaddr_in6 *ipv6_addr = nullptr;

	if (info->ai_family == AF_INET) {
		ipv4_addr = (struct sockaddr_in *)info->ai_addr;
		return &ipv4_addr->sin_addr;
	}
	else {
		ipv6_addr = (struct sockaddr_in6 *)info->ai_addr;
		return &ipv6_addr->sin6_addr;
	}

}

uint16_t InetPortInfo(const struct addrinfo *info) {
	struct sockaddr_in *ipv4_addr = nullptr;
	struct sockaddr_in6 *ipv6_addr = nullptr;

	if (info->ai_family == AF_INET) {
		ipv4_addr = (struct sockaddr_in *)info->ai_addr;
		return ipv4_addr->sin_port;
	}
	else {
		ipv6_addr = (struct sockaddr_in6 *)info->ai_addr;
		return ipv6_addr->sin6_port;
	}
}*/

void GetResolvedName(const struct addrinfo *addr, char *host, char *service) {
	int err;

	std::memset(host, 0, NI_MAXHOST);
	std::memset(service, 0, NI_MAXSERV);
	if ((err = getnameinfo(addr->ai_addr, sizeof(struct sockaddr_storage), host, NI_MAXHOST,
					service, NI_MAXSERV, NI_NUMERICSERV)))
		SysError("error: can't resolve name: " + std::string(gai_strerror(err)) + ": ");
}

void GetResolvedName(const struct sockaddr *addr, char *host, char *service) {
	int err;

	std::memset(host, 0, NI_MAXHOST);
	std::memset(service, 0, NI_MAXSERV);
	if ((err = getnameinfo(addr, sizeof(struct sockaddr_storage), host, NI_MAXHOST,
					service, NI_MAXSERV, NI_NUMERICSERV)))
		SysError("error: can't resolve name: " + std::string(gai_strerror(err)) + ": ");
}

int main(int argc, char *argv[]) {
	int seqNum, sock, numLen = std::to_string(INT_MIN).size() + 1;
	uint16_t port;
	char seqNumStr[numLen];
	char reqNumStr[numLen];
	char host[NI_MAXHOST];
	char srv[NI_MAXSERV];
	struct addrinfo hints;
	struct addrinfo *candi = nullptr, *roll = nullptr;
	std::string resolve;

	std::memset(seqNumStr, 0, numLen);
	std::strcpy(seqNumStr, (argc > 1) ? argv[1] : "0");
	seqNum = std::atoi(seqNumStr);
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		SysError("error: sigpipe: ");
	std::memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_NUMERICSERV | AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_next = nullptr;
	hints.ai_addr = nullptr;
	hints.ai_canonname = nullptr;
	int geta = getaddrinfo(nullptr, SRV_PORT, &hints, &candi);

	if (geta) {
		std::cerr << "error: can't get address sequence: "
			<< gai_strerror(geta) << std::endl;
		if (candi)
			freeaddrinfo(candi);
		return -1;
	}

	for (roll = candi; roll; roll = roll->ai_next) {
		int sockopt = 1;

		GetResolvedName(roll, host, srv);
 		sock = socket(roll->ai_family, roll->ai_socktype, roll->ai_protocol);
		if (sock == -1) {
			if (close(sock) == -1)
				SysError("error: can't close socket: ");
			ErrOutput("error: can't create socket: ");
			continue ;
		}
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) == -1) {
			if (close(sock) == -1)
				SysError("error: can't close socket: ");
			ErrOutput("error: can't set option for socket: ");
			continue ;
		}
		if (bind(sock, roll->ai_addr, roll->ai_addrlen) == -1) {
			if (close(sock) == -1)
				SysError("error: can't close socket: ");
			ErrOutput("error: can't bind socket to address (" + std::string(host)
						+ ", " + std::string(srv) + ": ");
			continue ;
		}
		else
			break ;
	}
	if (!roll) {
		freeaddrinfo(candi);
		std::cerr << "error: no address can be bound" << std::endl;
		return errno;
	}
	freeaddrinfo(candi);
	if (listen(sock, SOMAXCONN) == -1)
		SysError("error: can't create listening buffer: ");
	std::cout << "Lauched server on address (" + std::string(host) + ", " + std::string(srv) + ")" << std::endl;
	while(true) {
		struct sockaddr addr_cl;
		socklen_t len = sizeof(struct sockaddr_storage);
		ssize_t bytes;
		std::string aux;
		int cl;

		std::memset(reqNumStr, 0, numLen);
		cl = accept(sock, &addr_cl, &len);
		if (cl == -1)
			SysError("can't accept connection from client: ");
		GetResolvedName(&addr_cl, host, srv);
		while ((bytes = ReadLine(cl, reqNumStr, numLen)) > 0) {
			std::cout << "Server received " << numLen << " bytes from ("
				<< host << ", " << srv << ")" << std::endl;

			int reqNum = std::atoi(reqNumStr);
			if (reqNum < 0) {
				ErrOutput("error: negative numbers are not permitted: " + std::to_string(reqNum));
				memset(reqNumStr, 0, numLen);
				continue ;
			}
			std::cout << "received number: " << reqNumStr;
			aux += std::to_string(seqNum) + "\n";
			seqNum += reqNum - 1;
			std::memset(seqNumStr, 0, sizeof(seqNumStr));
			std::memset(reqNumStr, 0, sizeof(reqNumStr));
			strcpy(seqNumStr, std::to_string(seqNum).c_str());
		}
		aux += std::to_string(seqNum);
		if (bytes == -1)
			SysError("Error: can't read data from (" + std::string(host)
					 + ", " + std::string(srv) + ": ");
		bytes = write(cl, aux.c_str(), aux.size());
		if (!bytes)
			ErrOutput("no data is sent: buffer is empty");
		if (bytes == -1)
			SysError("error: can't send data to client (" + std::string(host)
					 + ", " + std::string(srv) + "): ");
		if (close(cl) == -1)
			SysError("error: can't close socket: ");
	}
	if (close(sock) == -1)
		SysError("error: can't close socket: ");
	return 0;
}
