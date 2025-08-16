cmake_minimum_required(VERSION 3.28)
project(karin_examples_draw_image)

set(CMAKE_CXX_STANDARD 20)

add_executable(karin_examples_draw_image main.cpp)
add_dependencies(karin_examples_draw_image karin_system karin_graphics)
target_link_libraries(karin_examples_draw_image PRIVATE karin_system karin_graphics)
target_include_directories(karin_examples_draw_image PRIVATE ${INCLUDE_DIR})

file(COPY square.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})