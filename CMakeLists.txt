cmake_minimum_required(VERSION 3.26)

project(aoc2025 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-Wall -Wextra -UNDEBUG)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(utils OBJECT src/utils.cpp)

function(prog prog_name)
  add_executable(${prog_name} "src/${prog_name}.cpp")
  target_link_libraries(${prog_name} utils)
endfunction()

prog(day01)
prog(day02)
