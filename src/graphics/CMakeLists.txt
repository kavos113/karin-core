set(GRAPHICS_SOURCE
        graphics_device.cpp
        renderer.cpp
        graphics_context.cpp
        path.cpp
        path_impl.cpp
        hash.cpp
        ${THIRD_PARTY_DIR}/stb_image/stb_image_impl.cpp
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
            vulkan/vulkan_graphics_device.cpp
            vulkan/vulkan_debug_manager.cpp
            vulkan/vulkan_utils.cpp
            vulkan/vulkan_surface.cpp
            vulkan/vulkan_renderer_impl.cpp
            vulkan/vulkan_graphics_context_impl.cpp
            vulkan/vulkan_pipeline.cpp
            vulkan/vulkan_tessellator.cpp
            vulkan/vulkan_device_resources.cpp
            vulkan/vulkan_glyph_cache.cpp
            vulkan/vma_impl.cpp
            text/font_loader.cpp
    )
endif ()

add_library(karin_graphics STATIC ${GRAPHICS_SOURCE})
target_include_directories(karin_graphics PUBLIC ${INCLUDE_DIR})
target_include_directories(karin_graphics PRIVATE ${COMMON_DIR})
target_include_directories(karin_graphics PRIVATE ${THIRD_PARTY_DIR})
target_include_directories(karin_graphics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


find_package(glm CONFIG REQUIRED)
target_link_libraries(karin_graphics PRIVATE glm::glm)
if (DIRECTX)
    target_link_libraries(karin_graphics PRIVATE d2d1.lib)
    target_link_libraries(karin_graphics PRIVATE dxgi.lib)
    target_link_libraries(karin_graphics PRIVATE d3d11.lib)
    target_link_libraries(karin_graphics PRIVATE dwrite.lib)

    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_DIRECTX)
    target_compile_definitions(karin_graphics PUBLIC UNICODE)
    target_compile_definitions(karin_graphics PUBLIC NOMINMAX)
    target_compile_definitions(karin_graphics PUBLIC WIN32_LEAN_AND_MEAN)
elseif (VULKAN)
    if (WIN32)
        find_program(
                BASH_EXECUTABLE bash
                HINTS "C:\\Program Files\\Git\\bin" "C:\\Program Files (x86)\\Git\\bin"
        )

        if (NOT BASH_EXECUTABLE)
            message(FATAL_ERROR "git bash has not found.")
        endif ()
    endif ()

    add_custom_command(
            OUTPUT vulkan/shaders/shaders.h
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMAND ${BASH_EXECUTABLE} ${SCRIPTS_DIR}/shader.sh vulkan/shaders SRC_GRAPHICS_VULKAN_SHADERS_SHADER_H
            DEPENDS
            vulkan/shaders/geometry.vert
            vulkan/shaders/geometry.frag
            vulkan/shaders/text.frag
            vulkan/shaders/push_constants.h
            COMMENT "Compiling graphics shaders"
    )
    add_custom_target(
            shaders_graphics ALL
            DEPENDS vulkan/shaders/shaders.h
    )

    add_dependencies(karin_graphics shaders_graphics)

    find_package(Vulkan REQUIRED)
    target_link_libraries(karin_graphics PRIVATE Vulkan::Vulkan)

    find_package(VulkanMemoryAllocator CONFIG REQUIRED)
    target_link_libraries(karin_graphics PRIVATE GPUOpen::VulkanMemoryAllocator)

    find_package(Freetype CONFIG REQUIRED)
    target_link_libraries(karin_graphics PRIVATE Freetype::Freetype)

    find_package(harfbuzz CONFIG REQUIRED)
    target_link_libraries(karin_graphics PRIVATE harfbuzz::harfbuzz)

    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_VULKAN)
endif ()

if (WIN32)
    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_WINDOWS)
elseif (APPLE)
    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_MACOS)
elseif (UNIX)
    target_compile_definitions(karin_graphics PUBLIC KARIN_PLATFORM_UNIX)
endif ()