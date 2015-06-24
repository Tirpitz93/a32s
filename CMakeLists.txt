cmake_minimum_required(VERSION 3.2)
project(arma_to_socket)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    arma-to-socket/arma-to-socket/arma-to-socket.cpp
    arma-to-socket/arma-to-socket/dllmain.cpp
    arma-to-socket/arma-to-socket/Socket.cpp
    arma-to-socket/arma-to-socket/Socket.h
    arma-to-socket/arma-to-socket/stdafx.cpp
    arma-to-socket/arma-to-socket/stdafx.h
    arma-to-socket/arma-to-socket/targetver.h
    arma-to-socket/debugconsole/debugconsole.cpp
    arma-to-socket/debugconsole/stdafx.cpp
    arma-to-socket/debugconsole/stdafx.h
    arma-to-socket/debugconsole/targetver.h)

add_executable(arma_to_socket ${SOURCE_FILES})