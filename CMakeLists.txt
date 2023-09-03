cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Cafe)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wnone")
set(CMAKE_CXX_FLAGS "-lstdc++fs")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

include_directories(src)

set(INCLUDE_SOURCE_FILES
        src/TokenHandler.cpp
        src/TokenHandler.h
        src/BiDirectionalMap.h
        )

add_executable(Cafe
        main.cpp
        ${INCLUDE_SOURCE_FILES}
        )

target_link_libraries(Cafe stdc++fs)
