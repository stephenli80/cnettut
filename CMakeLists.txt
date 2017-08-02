cmake_minimum_required(VERSION 3.7)
project(cnettut)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/ae_select.c
        src/ae.c
        src/config.c
        src/zmalloc.c
        src/anet.c
        src/networking.c
        src/sds.c
        src/server.c
        src/cnettut.c )

add_executable(cnettut ${SOURCE_FILES})



