#include "font_loader.h"

#include <stdexcept>

namespace karin
{
std::unique_ptr<IFontFace> FontLoader::loadFont(const Font& font)
{
    return m_platformLoader->loadFont(font);
}

std::vector<Font> FontLoader::getFontLists()
{
    return m_platformLoader->getFontLists();
}
} // karin