cmake_minimum_required(VERSION 3.19)
project(webserv)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(ast_calculator)

add_executable(webserv
        ast_calculator/astCalc.hpp)
