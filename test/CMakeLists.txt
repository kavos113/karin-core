cmake_minimum_required(VERSION 3.28)
project(karin_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(TEST_SRCS
        common/geometry/point_test.cpp
        common/geometry/size_test.cpp
        common/geometry/rectangle_test.cpp
        common/geometry/transform2d_test.cpp
        common/color/color_test.cpp
        common/utils/string_test.cpp
)

set(TEST_DEPEND_SRCS
        ${SOURCE_DIR}/common/geometry/transform2d.cpp
)

if (WIN32 AND VULKAN AND DIRECTX)
    set(TEST_SRCS ${TEST_SRCS}
        graphics/text/fontface_platform_test.cpp
    )
    set(TEST_DEPEND_SRCS ${TEST_DEPEND_SRCS}
        ${SOURCE_DIR}/graphics/text/windows/freetype_dwrite_font_loader.cpp
        ${SOURCE_DIR}/graphics/text/windows/dwrite_font_loader.cpp
        ${SOURCE_DIR}/graphics/text/windows/dwrite_font_face.cpp
        ${SOURCE_DIR}/graphics/text/freetype/freetype_font_face.cpp
        ${SOURCE_DIR}/graphics/d2d/d2d_context.cpp
    )
endif ()

file(COPY fixtures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(karin_test ${TEST_SRCS} ${TEST_DEPEND_SRCS})
target_link_libraries(karin_test
    PRIVATE
        GTest::gtest_main
)
target_include_directories(karin_test
    PRIVATE
        ${INCLUDE_DIR}
        ${SOURCE_DIR}/common
        ${SOURCE_DIR}/graphics
        ${SOURCE_DIR}/graphics/text
)
if (WIN32)
    target_compile_definitions(karin_test PUBLIC KARIN_PLATFORM_WINDOWS)

    find_package(harfbuzz CONFIG REQUIRED)
    target_link_libraries(karin_test
        PRIVATE
            dwrite
            d2d1
            d3d11
            harfbuzz::harfbuzz
    )
elseif (APPLE)
    target_compile_definitions(karin_test PUBLIC KARIN_PLATFORM_MACOS)
elseif (UNIX)
    target_compile_definitions(karin_test PUBLIC KARIN_PLATFORM_UNIX)
endif ()

if (MSVC)
    target_compile_options(karin_test PRIVATE /utf-8)
endif ()

find_package(glm CONFIG REQUIRED)
target_link_libraries(karin_test PRIVATE glm::glm)

include(GoogleTest)
gtest_discover_tests(karin_test)