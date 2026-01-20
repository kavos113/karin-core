#ifndef SRC_GRAPHICS_TEXT_PLATFORM_H
#define SRC_GRAPHICS_TEXT_PLATFORM_H

#include <memory>

#include "font_loader_platform.h"
#include "font_renderer_platform.h"

#ifdef KARIN_PLATFORM_DIRECTX
#include "windows/dwrite_font_loader.h"
#include "windows/dwrite_font_renderer.h"
#elifdef KARIN_PLATFORM_VULKAN
#ifdef KARIN_PLATFORM_WINDOWS
#include "windows/freetype_dwrite_font_loader.h"
#include "vulkan/vulkan_font_renderer.h"
#elifdef KARIN_PLATFORM_UNIX
#include "vulkan/vulkan_font_renderer.h"
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

inline std::unique_ptr<IPlatformFontRenderer> createFontRenderer()
{
#ifdef KARIN_PLATFORM_DIRECTX
    return std::make_unique<DwriteFontRenderer>();
#elifdef KARIN_PLATFORM_VULKAN
    return std::make_unique<VulkanFontRenderer>();
#endif
    return nullptr;
}
}

#endif //SRC_GRAPHICS_TEXT_PLATFORM_H