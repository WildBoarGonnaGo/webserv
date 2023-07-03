//
//  readline.hpp
//  webserv
//
//  Created by WildBoarGonnaGo on 27.06.2023.
//

#ifndef readline_hpp
#define readline_hpp

#include <string>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <iostream>

#define SRV_PORT "49600"
#define SERVICE_ECHO "echo"

ssize_t ReadLine(int fd, char *line, size_t n);
void ErrOutput(const std::string &prefix);
void SysError(const std::string &prefix);

#endif /* readline_hpp */
