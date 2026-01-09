set(TEXT_SOURCE
        text_renderer.cpp
        font_renderer.cpp
        font_layouter.cpp
        font_database.cpp
        font_loader.cpp
)


if (WIN32)
    set(TEXT_SOURCE ${TEXT_SOURCE}
            windows/dwrite_font_database.cpp
    )
elseif (LINUX)
endif ()

add_library(karin_graphics_text STATIC ${TEXT_SOURCE})
target_include_directories(karin_graphics_text PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})