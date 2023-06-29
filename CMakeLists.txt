cmake_minimum_required(VERSION 3.16)

project(webserv)

add_executable(${PROJECT_NAME} train/ipv4_tcp_server.cpp readline.cpp readline.hpp)
# add_subdirectory(client)
