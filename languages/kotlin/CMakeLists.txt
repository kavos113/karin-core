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

set_target_properties(karin_languages_java PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/native
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/native
)

find_package(yoga CONFIG REQUIRED)
target_link_libraries(karin_languages_java PRIVATE yoga::yogacore)