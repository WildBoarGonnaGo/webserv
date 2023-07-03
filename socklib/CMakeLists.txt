cmake_minimum_required(VERSION 3.16)

project(wildsock)

add_library(${PROJECT_NAME} SHARED socklib.h socklib.cpp)
