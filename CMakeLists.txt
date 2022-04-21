cmake_minimum_required(VERSION 3.21)
project(SocketFrameWork)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE config lib/config/*.cpp)
file(GLOB_RECURSE log lib/log/*.cpp)
file(GLOB_RECURSE thread lib/thread/*.cpp)
#file(GLOB_RECURSE event lib/event/*.cpp)

file(GLOB_RECURSE event lib/event/Channel.cpp
        lib/event/EventLoop.cpp
        lib/event/Poller.cpp
        lib/event/Timer.cpp
        lib/event/TimerManager.cpp
        lib/until/until.cpp
        lib/event/Acceptor.cpp
        lib/event/InetAddress.cpp
        lib/event/Socket.cpp
        lib/event/TcpConnection.cpp
        lib/event/TcpServer.cpp
        )

add_library(config SHARED ${config})
add_library(log SHARED ${log})
add_library(thread SHARED ${thread})
add_library(event SHARED ${event})



add_executable(SocketFrameWork main.cpp)
target_link_libraries(SocketFrameWork
        log
        config
        thread
        yaml-cpp
        pthread
        event
        )

add_executable(logger_test logger_test.cpp)
target_link_libraries(logger_test
        log
        config
        thread
        yaml-cpp
        pthread
        event
        )
