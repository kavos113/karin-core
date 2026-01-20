#ifndef SRC_GRAPHICS_TEXT_FONT_LOADER_H
#define SRC_GRAPHICS_TEXT_FONT_LOADER_H

#include <karin/system/font.h>
#include "font_loader_platform.h"
#include "font_face.h"

#include <memory>
#include <vector>

namespace karin
{
class FontLoader
{
public:
    std::unique_ptr<IFontFace> loadFont(const Font& font);
    std::vector<Font> getFontLists();

private:
    std::unique_ptr<IPlatformFontLoader> m_platformLoader;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FONT_LOADER_H