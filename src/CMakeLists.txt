macro(platform_definitions target_name)
    if (WIN32)
        target_compile_definitions(${target_name} PUBLIC KARIN_PLATFORM_WINDOWS)
        target_compile_definitions(${target_name} PUBLIC UNICODE)
    elseif (APPLE)
        target_compile_definitions(${target_name} PUBLIC KARIN_PLATFORM_MACOS)
    elseif (UNIX)
        target_compile_definitions(${target_name} PUBLIC KARIN_PLATFORM_UNIX)
    endif ()
endmacro()

macro(compile_glsl src_file dest_file bin_file target_name shader_target_name)
    add_custom_command(
            OUTPUT ${dest_file}
            COMMAND glslc ${src_file} -o ${dest_file}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${dest_file} ${bin_file}
            DEPENDS ${src_file}
            COMMENT "Compiling shader: ${src_file}"
    )
    add_custom_target(${shader_target_name} ALL
            DEPENDS ${dest_file}
    )
    add_dependencies(${target_name} ${shader_target_name})
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
            graphics/graphics/d2d/d2d_device_resources.cpp
    )
elseif (UNIX)
    set(GRAPHICS_SOURCE ${GRAPHICS_SOURCE}
            graphics/resources/vulkan/vk_graphics_device.cpp
            graphics/resources/vulkan/vk_debug_manager.cpp
            graphics/resources/vulkan/vk_device_utils.cpp
            graphics/resources/vulkan/vk_surface_impl.cpp
    )
endif ()

add_library(karin_graphics STATIC ${GRAPHICS_SOURCE})
target_include_directories(karin_graphics PUBLIC ${INCLUDE_DIR})
target_include_directories(karin_graphics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/graphics)
target_include_directories(karin_graphics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/common)
platform_definitions(karin_graphics)

if (WIN32)
    target_link_libraries(karin_graphics PRIVATE d2d1.lib)
    target_link_libraries(karin_graphics PRIVATE dxgi.lib)
    target_link_libraries(karin_graphics PRIVATE d3d11.lib)
elseif (UNIX)
    compile_glsl(
            ${CMAKE_CURRENT_SOURCE_DIR}/graphics/shaders/vert.vert
            ${CMAKE_CURRENT_SOURCE_DIR}/graphics/shaders/vert.spv
            ${CMAKE_CURRENT_BINARY_DIR}/shaders/vert.spv
            karin_graphics
            karin_graphics_vert
    )
    compile_glsl(
            ${CMAKE_CURRENT_SOURCE_DIR}/graphics/shaders/frag.frag
            ${CMAKE_CURRENT_SOURCE_DIR}/graphics/shaders/frag.spv
            ${CMAKE_CURRENT_BINARY_DIR}/shaders/frag.spv
            karin_graphics
            karin_graphics_frag
    )
endif ()