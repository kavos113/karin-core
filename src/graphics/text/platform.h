#ifndef SRC_GRAPHICS_TEXT_PLATFORM_H
#define SRC_GRAPHICS_TEXT_PLATFORM_H

#include <memory>

#include "font_loader_platform.h"

#ifdef KARIN_PLATFORM_DIRECTX
#include "windows/dwrite_font_loader.h"
#elifdef KARIN_PLATFORM_VULKAN
#ifdef KARIN_PLATFORM_WINDOWS
#include "windows/freetype_dwrite_font_loader.h"
#elifdef KARIN_PLATFORM_UNIX
#endif
#endif


namespace karin
{
inline std::unique_ptr<IPlatformFontLoader> createFontLoader()
{
#ifdef KARIN_PLATFORM_DIRECTX
    return std::make_unique<DwriteFontLoader>();
#elifdef KARIN_PLATFORM_VULKAN
#ifdef KARIN_PLATFORM_WINDOWS
    return std::make_unique<FreeTypeDWriteFontLoader>();
#elifdef KARIN_PLATFORM_UNIX

#endif
#endif
    return nullptr;
}
}

#endif //SRC_GRAPHICS_TEXT_PLATFORM_H