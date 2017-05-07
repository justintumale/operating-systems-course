cmake_minimum_required(VERSION 3.7)
project(simple_server_socket)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(simple_server_socket ${SOURCE_FILES})