cmake_minimum_required(VERSION 3.30)
project(gentle)

set(CMAKE_BUILD_TYPE Release)
include_directories(src)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})