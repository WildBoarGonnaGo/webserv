//
//  inet_udp_echo_sv.cpp
//  webserv
//
//  Created by WildBoarGonnaGo on 03.07.2023.
//

#include <readline.hpp>
#include <socklib.h>

int main(void) {
	socklen_t len;
	struct sockaddr cl;
	int sock = InetBind(std::string(SERVICE_ECHO), SOCK_DGRAM, len);
	if (sock == -1)
		return -1;

	while (true) {
		char buf[1024];
		char addr[FULL_INETSTRLEN];
		std::memset(buf, 0, 1024);
		ssize_t byte = recvfrom(sock, buf, 1024, 0, &cl, &len);

		if (byte == -1) {
			ErrOutput("error: can't read data from "
					  + std::string(InetAddressStr(&cl, len, addr, FULL_INETSTRLEN)) + ": ");
			continue ;
		}
		if (!byte)
			continue ;
		if (sendto(sock, buf, byte, 0, &cl, len) == -1) {
			ErrOutput("error: can't send data to " + std::string(addr) + ": ");
			continue ;
		}
	}
	return 0;
}
