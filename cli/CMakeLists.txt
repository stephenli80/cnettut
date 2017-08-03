cmake_minimum_required(VERSION 3.7)
project(cli)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/linenoise.c
        src/sds.c
        src/net.c
        src/dict.c
        src/ae.c
        src/client.c
        src/async.c
        src/cli.c)

add_executable(cli ${SOURCE_FILES})