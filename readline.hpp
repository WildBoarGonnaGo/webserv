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

ssize_t ReadLine(int fd, char *line);
void ErrOutput(const std::string &prefix);
void SysError(const std::string &prefix);

#endif /* readline_hpp */
