set(TEXT_SOURCE
        text_layouter.cpp
        font_loader.cpp
        text_engine.cpp
        text_engine_impl.cpp
)


if (WIN32)
    if (VULKAN)
        set(TEXT_SOURCE ${TEXT_SOURCE}
                windows/freetype_dwrite_font_loader.cpp
                freetype/freetype_font_face.cpp
        )
    elseif (DIRECTX)
        set(TEXT_SOURCE ${TEXT_SOURCE}
                windows/dwrite_font_loader.cpp
                windows/dwrite_font_face.cpp
        )
    endif ()

    set(TEXT_SOURCE ${TEXT_SOURCE}
            ${GRAPHICS_DIR}/d2d/d2d_context.cpp
    )
elseif (LINUX)
    set(TEXT_SOURCE ${TEXT_SOURCE}
            unix/freetype_fontconfig_font_loader.cpp
            freetype/freetype_font_face.cpp
    )
endif ()

add_library(karin_graphics_text STATIC ${TEXT_SOURCE})
target_include_directories(karin_graphics_text PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(karin_graphics_text PUBLIC ${INCLUDE_DIR})
target_include_directories(karin_graphics_text PRIVATE ${COMMON_DIR})
target_include_directories(karin_graphics_text PRIVATE ${GRAPHICS_DIR})

find_package(harfbuzz CONFIG REQUIRED)
target_link_libraries(karin_graphics_text PUBLIC harfbuzz::harfbuzz)

if (VULKAN)
    find_package(Freetype CONFIG REQUIRED)
    target_link_libraries(karin_graphics_text PUBLIC Freetype::Freetype)
endif ()

if (WIN32)
    target_link_libraries(karin_graphics_text PUBLIC d2d1 dwrite d3d11 dxgi)
endif ()

if (LINUX)
    find_package(Fontconfig REQUIRED)
    target_link_libraries(karin_graphics_text PRIVATE Fontconfig::Fontconfig)
endif ()

if (DIRECTX)
    target_compile_definitions(karin_graphics_text PUBLIC KARIN_PLATFORM_DIRECTX)
elseif (VULKAN)
    target_compile_definitions(karin_graphics_text PUBLIC KARIN_PLATFORM_VULKAN)
endif ()

if (WIN32)
    target_compile_definitions(karin_graphics_text PUBLIC KARIN_PLATFORM_WINDOWS)
elseif (LINUX)
    target_compile_definitions(karin_graphics_text PUBLIC KARIN_PLATFORM_UNIX)
endif ()