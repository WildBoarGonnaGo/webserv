cmake_minimum_required(VERSION 3.16)

project(client)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} inet_udp_echo_cl.cpp ../readline.cpp ../readline.hpp
	../socklib/socklib.h)
# add_subdirectory(/Volumes/KINGSTON/webserv/socklib)
target_include_directories(${PROJECT_NAME} PUBLIC ../ ../socklib)
target_link_directories(${PROJECT_NAME} PUBLIC ../socklib/xcode/Debug)
target_link_libraries(${PROJECT_NAME} -lwildsock)
