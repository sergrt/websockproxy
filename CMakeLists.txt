cmake_minimum_required(VERSION 3.15)

project(websockproxy VERSION 0.0.1 DESCRIPTION "Simple WebSocket HTTP server")

set(THIRDPARTY_DIR "${CMAKE_SOURCE_DIR}/3rdparty")

add_subdirectory(src)

add_subdirectory(test)
