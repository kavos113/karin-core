cmake_minimum_required(VERSION 3.28)
project(karin_examples_text_node)

set(CMAKE_CXX_STANDARD 20)

add_executable(karin_examples_text_node main.cpp)
add_dependencies(karin_examples_text_node
        karin_gui
        karin_system
        karin_graphics)
target_link_libraries(karin_examples_text_node PRIVATE
        karin_gui
        karin_system
        karin_graphics)
target_include_directories(karin_examples_text_node PRIVATE ${INCLUDE_DIR})
# for testing
target_include_directories(karin_examples_text_node PRIVATE ../../../src/gui)

find_package(yoga CONFIG REQUIRED)
target_link_libraries(karin_examples_text_node PRIVATE yoga::yogacore)