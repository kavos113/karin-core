cmake_minimum_required(VERSION 3.28)
set(VCPKG_MANIFEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/core)

project(karin)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/core/include)

add_subdirectory(core)
add_subdirectory(languages/kotlin)