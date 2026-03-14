#ifndef SRC_GRAPHICS_TEXT_FONT_LOADER_H
#define SRC_GRAPHICS_TEXT_FONT_LOADER_H

#include <karin/graphics/font.h>
#include <karin/graphics/font_face.h>
#include "font_loader_platform.h"

#include <memory>
#include <vector>

namespace karin
{
class FontLoader
{
public:
    FontLoader();
    ~FontLoader() = default;

    std::unique_ptr<IFontFace> loadFont(const Font& font);
    std::unique_ptr<IFontFace> loadFontFromFile(const std::string& filePath);
    std::vector<Font> getFontLists();

private:
    std::unique_ptr<IPlatformFontLoader> m_platformLoader;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FONT_LOADER_H