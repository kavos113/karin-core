#ifndef SRC_GRAPHICS_TEXT_FONT_LOADER_PLATFORM_H
#define SRC_GRAPHICS_TEXT_FONT_LOADER_PLATFORM_H

#include "font_face.h"
#include <karin/system/font.h>

#include <memory>
#include <vector>

namespace karin
{
class IPlatformFontLoader
{
public:
    virtual ~IPlatformFontLoader() = default;

    virtual std::unique_ptr<IFontFace> loadFont(const Font& font) = 0;
    virtual std::unique_ptr<IFontFace> loadFontFromFile(const std::string& filePath) = 0;
    virtual std::vector<Font> getFontLists() = 0;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FONT_LOADER_PLATFORM_H