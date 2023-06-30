cmake_minimum_required(VERSION 3.16)

project(webserv)
add_compile_options(-g)

add_executable(${PROJECT_NAME} train/ipv4v6_tcp_server.cpp readline.cpp readline.hpp)
# add_subdirectory(client)
