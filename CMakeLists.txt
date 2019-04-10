cmake_minimum_required(VERSION 3.12)

project(veriblockcore)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

include_directories(veriblock_core)

add_subdirectory(veriblock_core)
add_subdirectory(veriblock_test)