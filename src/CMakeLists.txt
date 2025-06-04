set(HEADER_FILES
        ${INCLUDE_DIR}/karin/system/application.h
        ${INCLUDE_DIR}/karin/system/window.h
        system/application_impl.h
        system/window_impl.h
        system/platform.h

        ${INCLUDE_DIR}/karin/common/point.h
        ${INCLUDE_DIR}/karin/common/size.h
        ${INCLUDE_DIR}/karin/common/rectangle.h
)

set(SOURCE_FILES
        system/application.cpp
        system/window.cpp
)

if (WIN32)
    set(SOURCE_FILES ${SOURCE_FILES}
            system/windows/win_application_impl.cpp
            system/windows/win_window_impl.cpp
            system/windows/win_window_class_registry.cpp
    )
    set(HEADER_FILES ${HEADER_FILES}
            system/windows/win_application_impl.h
            system/windows/win_window_impl.h
            system/windows/win_window_class_registry.h
    )
endif ()

add_library(karin_system STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(karin_system PUBLIC ${INCLUDE_DIR})
target_include_directories(karin_system PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/system)

if (WIN32)
    target_compile_definitions(karin_system PUBLIC KARIN_PLATFORM_WINDOWS)
    target_compile_definitions(karin_system PUBLIC UNICODE)
endif ()