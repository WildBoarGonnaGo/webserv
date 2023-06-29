//
//  readline.cpp
//  webserv
//
//  Created by WildBoarGonnaGo on 27.06.2023.
//

#include "readline.hpp"

void ErrOutput(const std::string &prefix) {
	std::cerr << prefix << strerror(errno) << std::endl;
}

void SysError(const std::string &prefix) {
	std::cerr << prefix << strerror(errno) << std::endl;
	exit errno;
}

ssize_t ReadLine(int fd, char *line, size_t n) {
	char *buf = line;
	size_t num = 0;
	char ch;
	char kek[2] = { 0 };

	if (n <= 0 || !line) {
		errno = EINVAL;
		return -1;
	}


	for (int i = 0; i < n - 1; ++i) {
		ssize_t byte = read(fd, kek, 1);
		kek[1] = '\0';
		if (byte == -1) {
			ErrOutput("error: ReadLine: can't read data: ");
			if (errno == EINTR) {
				std::cout << "ReadLine: restart file reading" << std::endl;
				continue ;
			}
			return -1;
		}
		else if (!byte)
			break ;
		else {
			if (i < n - 1) {
				++num;
				*(buf++) = kek[0];
			}
			if (kek[0] == '\n')
				break ;
		}
		kek[0] = '\0';
	}
	*buf = 0;
	return num;
}
