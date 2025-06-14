cmake_minimum_required(VERSION 3.28)
project(karin)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
if (WIN32)
    set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows)
elseif (LINUX)
    set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-linux)
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(test)