project(karin_java)

find_package(Java REQUIRED)
find_package(JNI REQUIRED)
add_library(karin_languages_java SHARED
        src/main/cpp/karin.cpp
        src/main/cpp/jni_global_ref.cpp
)

target_include_directories(karin_languages_java PRIVATE
    ${JNI_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/include
    ${CORE_INCLUDE_DIR}
)

target_link_libraries(karin_languages_java PRIVATE
    karin_system
    karin_graphics
    karin_gui
)
add_dependencies(karin_languages_java
    karin_system
    karin_graphics
    karin_gui
)

find_package(yoga CONFIG REQUIRED)
target_link_libraries(karin_languages_java PRIVATE yoga::yogacore)

if (WIN32)
    set(OS_DIR "windows")
    set(LIB_FILE "karin_languages_java.dll")
elseif (APPLE)
    set(OS_DIR "macos")
    set(LIB_FILE "libkarin_languages_java.dylib")
elseif (LINUX)
    set(OS_DIR "linux")
    set(LIB_FILE "libkarin_languages_java.so")
endif ()

set(LIB_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/main/resources/native/${OS_DIR}")

add_custom_command(TARGET karin_languages_java POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${LIB_OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:karin_languages_java>"
        "${LIB_OUTPUT_DIR}/${LIB_FILE}"
    COMMENT "Copying ${LIB_FILE} to ${LIB_OUTPUT_DIR}"
)