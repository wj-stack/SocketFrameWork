cmake_minimum_required(VERSION 3.21)
project(SocketFrameWork)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE config lib/config/*.cpp)
file(GLOB_RECURSE log lib/log/*.cpp)
file(GLOB_RECURSE thread lib/thread/*.cpp)

add_library(config SHARED ${config})
add_library(log SHARED ${log})
add_library(thread SHARED ${thread})



add_executable(SocketFrameWork main.cpp)
target_link_libraries(SocketFrameWork
        log
        config
        thread
        yaml-cpp
        pthread
        )

