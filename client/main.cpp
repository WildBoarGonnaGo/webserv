#include <sys/socket.h>
#include <iostream>
#include <sys/un.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <fstream>
#include <unistd.h>

void ErrOutput(std::string errPrefix) {
	std::cerr << errPrefix << strerror(errno) << std::endl;
	exit (errno);
}

int main() {
	struct sockaddr_un addr;
	const std::string path("/tmp/web.sock");
	int sock_cl;

	if ((sock_cl = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
		ErrOutput("error: can't create socket: ");
	std::memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	std::strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1);
	if ((connect(sock_cl, (struct sockaddr *)&addr, sizeof(struct sockaddr))) == -1)
		ErrOutput("error: can't connect to socket: " + std::string(addr.sun_path));
	while (true) {
		int b;
		std::string input;

		std::getline(std::cin, input);
		if ((b = write(sock_cl, input.c_str(), input.size())) == -1)
			ErrOutput("error: can't write to server: ");
		if (!b) {
			std::cout << "session is over" << std::endl;
			break ;
		}
	}
	if (close(sock_cl) == -1)
		ErrOutput("Can't close client socket: ");
	return 0;
}
