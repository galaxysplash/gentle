cmake_minimum_required(VERSION 3.25)
project(gentle)

include_directories(include)

add_subdirectory(modules)

file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE init mod_gen proj_gen)


