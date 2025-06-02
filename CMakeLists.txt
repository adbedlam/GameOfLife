cmake_minimum_required(VERSION 3.28)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

find_package(SDL2 CONFIG REQUIRED)

#include_directories(${SDL2_INCLUDE_DIRS})

add_executable(GameOfLife main.cpp)

target_link_libraries(GameOfLife PRIVATE SDL2::SDL2 SDL2::SDL2main)
