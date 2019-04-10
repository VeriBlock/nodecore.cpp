cmake_minimum_required(VERSION 3.12)

project(veriblockcore_test)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(src)
include_directories(../veriblock_core/src)
set(TEST_FILES src/TestCase.cpp)

add_executable(unit_tests ${TEST_FILES})

target_link_libraries(unit_tests veriblockcore_lib)