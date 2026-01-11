#ifndef SRC_GRAPHICS_TEXT_WINDOWS_DWRITE_FONT_LOADER_H
#define SRC_GRAPHICS_TEXT_WINDOWS_DWRITE_FONT_LOADER_H

#include <font_loader_platform.h>

namespace karin
{
class DwriteFontLoader : IPlatformFontLoader
{
    DwriteFontLoader() = default;
    ~DwriteFontLoader() override;

    std::unique_ptr<IFontFace> loadFont(const Font& font) override;
    std::vector<Font> getFontLists() override;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_WINDOWS_DWRITE_FONT_LOADER_H