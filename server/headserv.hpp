#ifndef HEADER_SERVER_HPP
# define HEADER_SERVER_HPP

// C++ libraries
#include <iostream>
#include <string>
#include <cstring>

// C libraries
#include <arpa/inet.h>
/*
** htonl
** htons
** ntohl
** ntohs
** inet_addr
*/

#include <sys/select.h>
/*
** select
*/

#include <pool.h>
/*
** pool
*/

#include <sys/epoll.h>
/*
** epool
*/

#include <sys/types.h>
/*
** kqueue
*/

#include <sys/socket.h>
/*
** socket
** accept
** listen
** send
** recv
** bind
** connect
** setsockopt
** getsockname
*/

#include <fcntl.h>
/*
** fcntl
*/

#define SOCKET_PORT	"8000"
#define SOCKET_IP	"127.0.0.1"


#endif
