macro(platform_definitions target_name)
    if (WIN32)
        target_compile_definitions(${target_name} PUBLIC KARIN_PLATFORM_WINDOWS)
        target_compile_definitions(${target_name} PUBLIC UNICODE)
        target_compile_definitions(${target_name} PUBLIC NOMINMAX)
    elseif (APPLE)
        target_compile_definitions(${target_name} PUBLIC KARIN_PLATFORM_MACOS)
    elseif (UNIX)
        target_compile_definitions(${target_name} PUBLIC KARIN_PLATFORM_UNIX)
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
elseif (UNIX)
    set(SYSTEM_SOURCE ${SYSTEM_SOURCE}
            system/x11/x11_application_impl.cpp
            system/x11/x11_window_impl.cpp
    )
endif ()

add_library(karin_system STATIC ${SYSTEM_SOURCE})
target_include_directories(karin_system PUBLIC ${INCLUDE_DIR})
target_include_directories(karin_system PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/system)
target_include_directories(karin_system PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/common)
platform_definitions(karin_system)

if (UNIX)
    target_link_libraries(karin_system PRIVATE X11)
endif ()

# ----- Graphics modules -----
set(GRAPHICS_SOURCE
        graphics/graphics_device.cpp
        graphics/renderer.cpp
        graphics/graphics_context.cpp
        graphics/path.cpp
        graphics/path_impl.cpp
)

if (WIN32)
    set(GRAPHICS_SOURCE ${GRAPHICS_SOURCE}
            graphics/d2d/d2d_graphics_device.cpp
            graphics/d2d/d2d_surface_manager.cpp
            graphics/d2d/d2d_renderer_impl.cpp
            graphics/d2d/d2d_graphics_context_impl.cpp
            graphics/d2d/d2d_device_resources.cpp
    )
elseif (UNIX)
    set(GRAPHICS_SOURCE ${GRAPHICS_SOURCE}
            graphics/vulkan/vk_graphics_device.cpp
            graphics/vulkan/vk_debug_manager.cpp
            graphics/vulkan/vk_utils.cpp
            graphics/vulkan/vk_surface_manager.cpp
            graphics/vulkan/vk_renderer_impl.cpp
            graphics/vulkan/vk_graphics_context_impl.cpp
            graphics/vulkan/vk_pipeline_manager.cpp
            graphics/vulkan/vma_impl.cpp
    )
endif ()

add_library(karin_graphics STATIC ${GRAPHICS_SOURCE})
target_include_directories(karin_graphics PUBLIC ${INCLUDE_DIR})
target_include_directories(karin_graphics PRIVATE ${THIRD_PARTY_DIR}/include)
target_include_directories(karin_graphics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/graphics)
target_include_directories(karin_graphics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/common)
platform_definitions(karin_graphics)

if (WIN32)
    target_link_libraries(karin_graphics PRIVATE d2d1.lib)
    target_link_libraries(karin_graphics PRIVATE dxgi.lib)
    target_link_libraries(karin_graphics PRIVATE d3d11.lib)
elseif (UNIX)
    add_custom_command(
            OUTPUT graphics/vulkan/shaders/shaders.h
            COMMAND ${SCRIPTS_DIR}/shader.sh graphics/vulkan/shaders SRC_GRAPHICS_SHADERS_SHADER_H
            DEPENDS
            graphics/vulkan/shaders/shader.vert
            graphics/vulkan/shaders/shader.frag
            COMMENT "Compiling graphics shaders"
    )
    add_custom_target(
            shaders_graphics ALL
            DEPENDS graphics/vulkan/shaders/shaders.h
    )

    add_dependencies(karin_graphics shaders_graphics)

    target_link_libraries(karin_graphics PRIVATE vulkan)
endif ()