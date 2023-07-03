//
//  inet_udp_echo_cl.cpp
//  client
//
//  Created by WildBoarGonnaGo on 03.07.2023.
//

#include <readline.hpp>
#include <socklib.h>

int main(int argc, char *argv[]) {
	if (argc < 3 || (argc == 2 && std::string(argv[1]) == "--help")) {
		std::cout << "Usage: client <host> <str1> <str2> ..." << std::endl;
		return 0;
	}

	std::string host = argv[1];
	std::string srv = SERVICE_ECHO;
	int sock = InetConnect(host, srv, SOCK_DGRAM);
	if (sock == -1)
		return -1;
	for (int i = 2; i < argc; ++i) {
		char addr[FULL_INETSTRLEN];
		char buf[1024];
		ssize_t bytes = write(sock, argv[i], std::strlen(argv[i]));

		if (bytes == -1) {
			ErrOutput("error: can't send data to (" + std::string(argv[1]) + ", "
					  + std::string(SERVICE_ECHO) + "): ");
			continue;
		}
		std::memset(buf, 0, 1024);
		bytes = read(sock, buf, 1024);
		if (bytes == -1) {
			ErrOutput("error: can't read data from (" + std::string(argv[1]) + ", "
					  + std::string(SERVICE_ECHO) + "): ");
			continue;
		}
		else if (!bytes) {
			std::cout << "no data was received" << std::endl;
			continue ;
		}
		std::cout << bytes << " bytes was received from server (" << argv[1]
			<< ", " << SERVICE_ECHO << "): " << buf << std::endl;
	}
	if (close(sock) == -1)
		SysError("error: can't close socket");
	return 0;
}
