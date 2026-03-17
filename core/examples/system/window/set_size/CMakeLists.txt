cmake_minimum_required(VERSION 3.28)
project(karin_examples_set_size)

set(CMAKE_CXX_STANDARD 20)

add_executable(karin_examples_set_size main.cpp)
add_dependencies(karin_examples_set_size karin_system)
target_link_libraries(karin_examples_set_size PRIVATE karin_system)
target_include_directories(karin_examples_set_size PRIVATE ${INCLUDE_DIR})