#include "socklib.h"
#include <iostream>
#include <unistd.h>
#include <cerrno>

int InetConnect(const std::string &host, const std::string &service, const int &soType) {
	int sock, err;
	struct addrinfo hints;
	struct addrinfo *it = nullptr, *lst = nullptr;

	std::memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = soType;
	hints.ai_flags = AI_ALL | AI_V4MAPPED_CFG;

	err = getaddrinfo(host.c_str(), service.c_str(), &hints, &lst);
	if (err) {
		std::cerr << "error: can't get address list: " << gai_strerror(err) << std::endl;
		return -1;
	}
	for (it = lst; it; it = it->ai_next) {
		sock = socket(it->ai_family, it->ai_socktype, it->ai_protocol);
		if (sock == -1) {
			std::cerr << "error: can't create socket: " << strerror(errno)
				<< std::endl;
			continue;;
		}
		if (connect(sock, it->ai_addr, it->ai_addrlen) == -1) {
			if (close(sock) == -1) {
				std::cerr << "error: can't close socket: " << strerror(errno)
					<< std::endl;
				continue;;
			}
			std::cerr << "error: connect to address (" << host << ", "
				<< service << "): " << std::endl;
			continue;;
		}
		break ;
	}
	if (!it) {
		freeaddrinfo(lst);
		std::cerr << "error: can't connect any of listed addresses." << std::endl;
		return -1;
	}
	freeaddrinfo(lst);
	return sock;
}

int InetListen(const std::string &service, const int &backlog, socklen_t &addrlen) {
	int sock, err;
	struct addrinfo hints;
	struct addrinfo *it, *lst;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE | AI_ALL | AI_V4MAPPED_CFG;

	err = getaddrinfo(nullptr, service.c_str(), &hints, &lst);
	if (err) {
		std::cerr << "error: can't get address list: " << gai_strerror(err) << std::endl;
		return -1;
	}
	for (it = lst; it; it = it->ai_next) {
		sock = socket(it->ai_family, it->ai_socktype, it->ai_protocol);
		if (sock == -1) {
			std::cerr << "error: can't create socket: " << strerror(errno)
				<< std::endl;
			continue ;
		}
		if (bind(sock, it->ai_addr, it->ai_addrlen) == -1) {
			if (close(sock) == -1) {
				std::cerr << "error: can't close socket: " << strerror(errno) << std::endl;
				continue ;
			}
			char name[FULL_INETSTRLEN];
			std::cerr << "error: can't bind socket to address " << InetAddressStr(it->ai_addr, it->ai_addrlen, name, FULL_INETSTRLEN)
				<< ": " << strerror(errno) << std::endl;
			continue ;
		}
		if (listen(sock, backlog) == -1) {
			if (close(sock) == -1) {
				std::cerr << "error: can't close socket: " << strerror(errno) << std::endl;
				continue;
			}
			char name[FULL_INETSTRLEN];
			std::cerr << "error: address " << InetAddressStr(it->ai_addr, it->ai_addrlen, name, FULL_INETSTRLEN)
				<< "can't listen to clients: " << strerror(errno) << std::endl;
			continue;
		}
		break ;
	}
	if (!it) {
		freeaddrinfo(lst);
		std::cerr << "error: can't bind socket to any of address" << std::endl;
		return -1;
	}
	addrlen = it->ai_addrlen;
	freeaddrinfo(lst);
	return sock;
}

int InetBind(const std::string &service, const int &type, socklen_t &addrlen) {
	int sock, err;
	struct addrinfo hints;
	struct addrinfo *it, *lst;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE | AI_ALL | AI_V4MAPPED_CFG;
	hints.ai_socktype = type;

	err = getaddrinfo(nullptr, service.c_str(), &hints, &lst);
	if (err) {
		std::cerr << "error: can't get address list: " << gai_strerror(err) << std::endl;
		return -1;
	}
	for (it = lst; it; it = it->ai_next) {
		sock = socket(it->ai_family, it->ai_socktype, it->ai_protocol);
		if (sock == -1) {
			std::cerr << "error: can't create socket: " << strerror(errno)
				<< std::endl;
			continue ;
		}
		if (bind(sock, it->ai_addr, it->ai_addrlen) == -1) {
			if (close(sock) == -1) {
				std::cerr << "error: can't close socket: " << strerror(errno) << std::endl;
				continue ;
			}
			char name[FULL_INETSTRLEN];
			std::cerr << "error: can't bind socket to address " << InetAddressStr(it->ai_addr, it->ai_addrlen, name, FULL_INETSTRLEN)
				<< ": " << strerror(errno) << std::endl;
			continue ;
		}
		break ;
	}
	if (!it) {
		freeaddrinfo(lst);
		std::cerr << "error: can't bind socket to any of address" << std::endl;
		return -1;
	}
	addrlen = it->ai_addrlen;
	freeaddrinfo(lst);
	return sock;
}

char *InetAddressStr(const struct sockaddr *addr, socklen_t &addrlen,
					 char *addrStr, int addrStrLen) {
	char host[NI_MAXHOST] = { 0 };
	char srv[NI_MAXSERV] = { 0 };
	char aux[3] = ", ";
	char *itH = host, *itS = srv, *itA = aux;

	assert(addrStr);
	std::memset(addrStr, 0, addrStrLen);
	int err = getnameinfo(addr, addrlen, host, NI_MAXHOST, srv, NI_MAXSERV, NI_NUMERICSERV);
	if (err) {
		std::cerr << "error: can't resolve address: " << gai_strerror(err) << std::endl;
		return addrStr;
	}
	for (int i = 0; i < addrStrLen; ++i) {
		if (!i)
			addrStr[i] = '(';
		else if (*itH)
			addrStr[i] = *(itH++);
		else if (*itA)
			addrStr[i] = *(itA++);
		else if (*itS)
			addrStr[i] = *(itS++);
		else {
			addrStr[i] = ')';
			break ;
		}
	}
	return addrStr;
}
