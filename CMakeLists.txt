cmake_minimum_required(VERSION 3.15)
project(os1 C)

set(CMAKE_C_STANDARD 11)

add_executable(os1 main.c memory.c memory.h)