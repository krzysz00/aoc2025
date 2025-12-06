cmake_minimum_required(VERSION 3.26)

project(aoc2025 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(day01 src/day01.cpp)
