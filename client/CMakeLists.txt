cmake_minimum_required(VERSION 3.16)

project(client)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} ipv4_udp_client.cpp)
