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
	ssize_t num = 0;
	char ch;

	if (n <= 0 || !line) {
		errno = EINVAL;
		return -1;
	}

	for (size_t i = 0; i < n - 1; ++i) {
		ssize_t byte = read(fd, &ch, 1);
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
				*(buf++) = ch;
			}
			if (ch == '\n')
				break ;
		}
	}
	*buf = 0;
	return num;
}
