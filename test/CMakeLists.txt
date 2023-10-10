cmake_minimum_required(VERSION 3.10)

project(websockproxy_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ${THIRDPARTY_DIR}/cpp-httplib
    ${THIRDPARTY_DIR}/json/include
    ${CMAKE_SOURCE_DIR}/src
    ${gtest_SOURCE_DIR}/include
    ${gtest_SOURCE_DIR}
)

set(SOURCE
    JsonParse.cpp
    main.cpp
    RequestsParse.cpp
    UnityBuild.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

IF (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE)
ENDIF()

add_subdirectory(${THIRDPARTY_DIR}/gtest test/)
enable_testing()

link_directories(
    ${CMAKE_BINARY_DIR}/src/${CMAKE_BUILD_TYPE}
    ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}
)

target_link_libraries(${PROJECT_NAME} gtest)
