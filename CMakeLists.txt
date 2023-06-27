cmake_minimum_required(VERSION 3.16)

project(webserv)

add_executable(${PROJECT_NAME} train/ipv6_udp_server.cpp)
# add_subdirectory(client)
