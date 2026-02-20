#include "font_loader.h"

#include <stdexcept>

namespace karin
{
std::unique_ptr<IFontFace> FontLoader::loadFont(const Font& font)
{
    return m_platformLoader->loadFont(font);
}

std::unique_ptr<IFontFace> FontLoader::loadFontFromFile(const std::string& filePath)
{
    return m_platformLoader->loadFontFromFile(filePath);
}

std::vector<Font> FontLoader::getFontLists()
{
    return m_platformLoader->getFontLists();
}
} // karin