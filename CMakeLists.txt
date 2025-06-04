cmake_minimum_required(VERSION 3.28)
project(karin)

set(CMAKE_CXX_STANDARD 20)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(test)