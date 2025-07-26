set(GRAPHICS_SOURCE
        graphics_device.cpp
        renderer.cpp
        graphics_context.cpp
        path.cpp
        path_impl.cpp
)

if (DIRECTX)
    set(GRAPHICS_SOURCE ${GRAPHICS_SOURCE}
            d2d/d2d_graphics_device.cpp
            d2d/d2d_surface_manager.cpp
            d2d/d2d_renderer_impl.cpp
            d2d/d2d_graphics_context_impl.cpp
            d2d/d2d_device_resources.cpp
    )
elseif (VULKAN)
    set(GRAPHICS_SOURCE ${GRAPHICS_SOURCE}
            vulkan/vk_graphics_device.cpp
            vulkan/vk_debug_manager.cpp
            vulkan/vk_utils.cpp
            vulkan/vk_surface_manager.cpp
            vulkan/vk_renderer_impl.cpp
            vulkan/vk_graphics_context_impl.cpp
            vulkan/vk_pipeline_manager.cpp
            vulkan/vma_impl.cpp
    )
endif ()

add_library(karin_graphics STATIC ${GRAPHICS_SOURCE})
target_include_directories(karin_graphics PUBLIC ${INCLUDE_DIR})
target_include_directories(karin_graphics PRIVATE ${THIRD_PARTY_DIR}/include)
target_include_directories(karin_graphics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/graphics)
target_include_directories(karin_graphics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/common)

if (DIRECTX)
    target_link_libraries(karin_graphics PRIVATE d2d1.lib)
    target_link_libraries(karin_graphics PRIVATE dxgi.lib)
    target_link_libraries(karin_graphics PRIVATE d3d11.lib)
    
    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_DIRECTX)
    target_compile_definitions(karin_graphics PUBLIC UNICODE)
    target_compile_definitions(karin_graphics PUBLIC NOMINMAX)
elseif (VULKAN)
    add_custom_command(
            OUTPUT vulkan/shaders/shaders.h
            COMMAND ${SCRIPTS_DIR}/shader.sh vulkan/shaders SRC_GRAPHICS_VULKAN_SHADERS_SHADER_H
            DEPENDS
            vulkan/shaders/shader.vert
            vulkan/shaders/shader.frag
            COMMENT "Compiling graphics shaders"
    )
    add_custom_target(
            shaders_graphics ALL
            DEPENDS vulkan/shaders/shaders.h
    )

    add_dependencies(karin_graphics shaders_graphics)

    target_link_libraries(karin_graphics PRIVATE vulkan)
    
    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_VULKAN)
endif ()

if (WIN32)
    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_WINDOWS)
elseif (APPLE)
    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_MACOS)
elseif (UNIX)
    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_UNIX)
endif ()