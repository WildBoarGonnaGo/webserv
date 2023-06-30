//
//  ipv4_tcp_client.cpp
//  client
//
//  Created by WildBoarGonnaGo on 29.06.2023.
//

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "../readline.hpp"
#include <netdb.h>
#include <cstdlib>
#include <climits>

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

	if (argc < 2 || (argc == 2 && !strcmp(argv[1], "--help"))) {
		std::cout << "Usage: <hostname> <number>" << std::endl;
		return 0;
	}

	struct addrinfo hints;
	struct addrinfo *it, *cli;
	int sock, numLen = std::to_string(INT_MIN).size() + 1;
	char host[NI_MAXHOST] = { 0 };
	char srv[NI_MAXSERV] = { 0 };
	char num[std::to_string(INT_MIN).size() + 1];

	std::memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_NUMERICSERV;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_canonname = nullptr;
	hints.ai_next = nullptr;
	hints.ai_addr = nullptr;

	int resolveResult = getaddrinfo(argv[1], SRV_PORT, &hints, &cli);
	if (resolveResult) {
		std::cerr << "error: can't resolve address list: "
			<< gai_strerror(resolveResult) << std::endl;
		return resolveResult;
	}

	for (it = cli; it; it = it->ai_next) {
		int sockopt;

		GetResolvedName(it, host, srv);
		sock = socket(it->ai_family, it->ai_socktype, it->ai_protocol);
		if (sock == -1) {
			ErrOutput("error: can't create socket: ");
			continue ;
		}
		if (connect(sock, it->ai_addr, it->ai_addrlen) == -1) {
			if (close(sock) == -1)
				SysError("error: can't close socket");
			ErrOutput("error: can't connect to server (" + std::string(host)
					  + ", " + std::string(srv) + "): ");
			continue ;
		}
		else
			break ;
	}
	if (!it) {
		freeaddrinfo(cli);
		std::cerr << "error: can't establish connection between client and server" << std::endl;
		return -1;
	}
	freeaddrinfo(cli);
	int seq = (argc > 2) ? std::atoi(argv[2]) : 1;
	/*if (errno) {
		if (close(sock) == -1)
			SysError("error: can't close socket: ");
		SysError("error: atoi: ");
	}*/
	if (seq < 1) {
		std::cerr << "error: " << seq << ": sequence number can't be less than 1";
		if (close(sock) == -1)
			SysError("error: can't close socket: ");
	}
	std::string req;
	ssize_t bytes;
	req += std::string(argv[2]) + "\n";
	for (int i = 0; i < seq; ++i) {
		std::string auxNum;
		std::memset(num, 0, numLen);
		std::cout << "request number " << i + 1 << ": ";
		std::cin >> auxNum;
		//bytes = ReadLine(STDIN_FILENO, num, numLen);
		//if (bytes == -1)
		//	SysError("error: can't read data from input filestream: ");
		req += auxNum + "\n";
	}
	//req[req.size() - 1] = '\0';
	bytes = write(sock, req.c_str(), req.size());
	if (bytes == -1) {
		if (close(sock) == -1)
			SysError("error: can't close socket: ");
		SysError("error: can't send data to server (" + std::string(host)
				 + ", " + std::string(srv) + ": ");
	}
	std::memset(num, 0, numLen);
	int c = 0, thrs = seq + 1;
	while (c < thrs && (bytes = ReadLine(sock, num, numLen)) > 0) {
		std::cout << "Sequence number " << ++c << ": " << num;
		std::memset(num, 0, numLen);
	}
	if (bytes == -1) {
		if (close(sock) == -1)
			SysError("error: can't close socket: ");
		SysError("error: can't read data from server (" + std::string(host)
				 + ", " + std::string(srv) + ": ");
	}
	if (close(sock) == -1)
		SysError("error: can't close socket: ");
	return 0;
}
