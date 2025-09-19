cmake_minimum_required(VERSION 4.0)
project(myRedis)

set(CMAKE_CXX_STANDARD 20)

add_executable(myRedis main.cpp
        include/server.h
        src/server.cpp
        include/parsing.h
        src/parsing.cpp
        tests/tserver.cpp
        tests/tparsing.cpp
        include/command_handler.h
        src/command_handler.cpp)