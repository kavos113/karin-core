set(SYSTEM_SOURCE
        application.cpp
        window.cpp
        system_font.cpp
)

if (WIN32)
    set(SYSTEM_SOURCE ${SYSTEM_SOURCE}
            windows/win_application_impl.cpp
            windows/win_window_impl.cpp
            windows/win_window_class_registry.cpp
            windows/win_event.cpp
            windows/win_converter.cpp
            windows/win_system_font_impl.cpp
    )
elseif (UNIX)
    set(SYSTEM_SOURCE ${SYSTEM_SOURCE}
            x11/x11_application_impl.cpp
            x11/x11_window_impl.cpp
    )
endif ()

add_library(karin_system STATIC ${SYSTEM_SOURCE})
target_include_directories(karin_system PUBLIC ${INCLUDE_DIR})
target_include_directories(karin_system PUBLIC ${COMMON_DIR})
target_include_directories(karin_system PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
    target_compile_definitions(karin_system PUBLIC KARIN_PLATFORM_WINDOWS)
    target_compile_definitions(karin_system PUBLIC UNICODE)
    target_compile_definitions(karin_system PUBLIC NOMINMAX)
    target_compile_definitions(karin_system PUBLIC WIN32_LEAN_AND_MEAN)
elseif (APPLE)
    target_compile_definitions(karin_system PUBLIC KARIN_PLATFORM_MACOS)
elseif (UNIX)
    target_compile_definitions(karin_system PUBLIC KARIN_PLATFORM_UNIX)
endif ()

if (UNIX)
    target_link_libraries(karin_system PRIVATE X11)
endif ()