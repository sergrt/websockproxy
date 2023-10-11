cmake_minimum_required(VERSION 3.15)

project(websockproxy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("${THIRDPARTY_DIR}/asio/asio/include")
include_directories("${THIRDPARTY_DIR}/cpp-httplib")
include_directories("${THIRDPARTY_DIR}/Crow/include")
include_directories("${THIRDPARTY_DIR}/json/include")

set(SOURCE
    HttpClient.cpp
    main.cpp
    Requests.cpp
    WsServer.cpp
)

set(HEADER
    HttpClient.h
    Requests.h
    Response.h
    Method.h
    WsServer.h
)

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADER})
