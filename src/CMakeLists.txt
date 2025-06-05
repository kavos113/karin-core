macro(platform_definitions target_name)
    if (WIN32)
        target_compile_definitions(${target_name} PUBLIC KARIN_PLATFORM_WINDOWS)
        target_compile_definitions(${target_name} PUBLIC UNICODE)
    elseif (APPLE)
        target_compile_definitions(${target_name} PUBLIC KARIN_PLATFORM_MACOS)
    elseif (UNIX)
        target_compile_definitions(${target_name} PUBLIC KARIN_PLATFORM_LINUX)
    endif ()
endmacro()

# ----- System modules -----
set(SYSTEM_SOURCE
        system/application.cpp
        system/window.cpp
)

if (WIN32)
    set(SYSTEM_SOURCE ${SYSTEM_SOURCE}
            system/windows/win_application_impl.cpp
            system/windows/win_window_impl.cpp
            system/windows/win_window_class_registry.cpp
    )
endif ()

add_library(karin_system STATIC ${SYSTEM_SOURCE})
target_include_directories(karin_system PUBLIC ${INCLUDE_DIR})
target_include_directories(karin_system PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/system)
platform_definitions(karin_system)

# ----- Graphics modules -----
set(GRAPHICS_SOURCE
        graphics/resources/graphics_device.cpp
        graphics/graphics/renderer.cpp
        graphics/graphics/graphics_context.cpp
)

if (WIN32)
    set(GRAPHICS_SOURCE ${GRAPHICS_SOURCE}
            graphics/resources/d2d/d2d_graphics_device.cpp
            graphics/resources/d2d/d2d_surface_impl.cpp
            graphics/graphics/d2d/d2d_renderer_impl.cpp
            graphics/graphics/d2d/d2d_graphics_context_impl.cpp
    )
endif ()

add_library(karin_graphics STATIC ${GRAPHICS_SOURCE})
target_include_directories(karin_graphics PUBLIC ${INCLUDE_DIR})
target_include_directories(karin_graphics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/graphics)
platform_definitions(karin_graphics)

if (WIN32)
    target_link_libraries(karin_graphics PRIVATE d2d1.lib)
    target_link_libraries(karin_graphics PRIVATE dxgi.lib)
    target_link_libraries(karin_graphics PRIVATE d3d11.lib)
endif ()