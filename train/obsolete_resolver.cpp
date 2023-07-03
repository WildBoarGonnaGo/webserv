#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <cerrno>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cout << "Usage: resolve <hostname 1> <hostname 2> ..." << std::endl;
		return -1;
	}

	char **pp = nullptr;
	char addr[INET6_ADDRSTRLEN] = { 0 };

	for (int i = 1; i < argc; ++i) {
		std::cout << "target: " << argv[i] << std::endl;
		struct hostent *res = gethostbyname(argv[i]);
		if (!res) {
			std::cerr << "error: can't resolver hostname: " << hstrerror(h_errno) << std::endl;
			return -1;
		}
		std::cout << "cannonical name: " << res->h_name << std::endl;
		std::cout << "aliases names: ";
		for (pp = res->h_aliases; *pp; ++pp) {
			std::cout << *pp;
			char **next = pp; ++next;
			if (next && *next)
				std::cout << ", ";  
		}
		std::cout << std::endl;
		std::cout << "address type: " << ((res->h_addrtype == AF_INET) ? "AF_INET"
			: (res->h_addrtype == AF_INET6) ? "AF_INET6" : "<unknown>") << std::endl;
		if (res->h_addrtype == AF_INET || res->h_addrtype == AF_INET6) {
			std::cout << "addresses: ";
			for (pp = res->h_addr_list; *pp; ++pp) {
				std::memset(addr, 0, INET6_ADDRSTRLEN);
				if (!inet_ntop(res->h_addrtype, *pp, addr, INET6_ADDRSTRLEN)) {
					std::cerr << "can't get address: " << strerror(errno) << std::endl;
					exit errno;
				}
				std::cout << addr;
				char **next = pp; ++next;
				if (next && *next)
					std::cout << ", ";
			}
			std::cout << std::endl;
		}
		if (i < argc - 1) std::cout << std::endl;
	}
	return 0;
}
