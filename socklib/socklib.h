#ifndef SOCKLIB_H
#define SOCKLIB_H

#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <cstring>
#include <sys/types.h>

#define FULL_INETSTRLEN NI_MAXHOST + NI_MAXSERV + 10

int InetConnect(const std::string &host, const std::string &service, const int &soType);
int InetListen(const std::string &service, const int &backlog, socklen_t &addrlen);
int InetBind(const std::string &service, const int &type, socklen_t &addrlen);
char *InetAddressStr(const struct sockaddr *addr, socklen_t &addrlen,
					 char *addrStr, int addrStrLen);

#endif
