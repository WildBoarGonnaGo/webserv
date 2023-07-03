cmake_minimum_required(VERSION 3.16)

project(webserv)
add_compile_options(-g)

add_executable(${PROJECT_NAME} train/inet_udp_echo_sv.cpp readline.cpp readline.hpp)
add_subdirectory(socklib)
target_include_directories(${PROJECT_NAME} PUBLIC ./socklib .)
target_link_directories(${PROJECT_NAME} PUBLIC ./socklib/xcode/Debug)
target_link_libraries(${PROJECT_NAME} -lwildsock)
