cmake_minimum_required(VERSION 3.20)

project(webserv)

add_executable(${PROJECT_NAME} train/unix_socket.cpp)
add_subdirectory(client)
