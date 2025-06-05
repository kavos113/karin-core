cmake_minimum_required(VERSION 3.28)
project(karin_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(TEST_SRCS
        common/geometry/point_test.cpp
        common/geometry/size_test.cpp
        common/geometry/rectangle_test.cpp
        common/color/color_test.cpp
)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(karin_test ${TEST_SRCS})
add_dependencies(karin_test karin_system)
target_link_libraries(karin_test
    PRIVATE
        karin_system
        GTest::gtest_main
)
target_include_directories(karin_test
    PRIVATE
        ${INCLUDE_DIR}
)

include(GoogleTest)
gtest_discover_tests(karin_test)