set(TEXT_SOURCE
        text_renderer.cpp
        font_renderer.cpp
        font_layouter.cpp
        font_loader.cpp
)


if (WIN32)
    if (VULKAN)
        set(TEXT_SOURCE ${TEXT_SOURCE}
                windows/freetype_dwrite_font_loader.cpp
                vulkan/vulkan_font_renderer.cpp
        )
    elseif (DIRECTX)
        set(TEXT_SOURCE ${TEXT_SOURCE}
                windows/dwrite_font_renderer.cpp
                windows/dwrite_font_loader.cpp
        )
    endif ()
elseif (LINUX)
    set(TEXT_SOURCE ${TEXT_SOURCE}
            unix/freetype_fontconfig_font_loader.cpp
            vulkan/vulkan_font_renderer.cpp
    )
endif ()

add_library(karin_graphics_text STATIC ${TEXT_SOURCE})
target_include_directories(karin_graphics_text PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})