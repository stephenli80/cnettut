cmake_minimum_required(VERSION 3.7)
project(cli)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/cli.c)

add_executable(cli ${SOURCE_FILES})