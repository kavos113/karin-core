#include "font_loader.h"

#include "platform.h"

#include <stdexcept>

namespace karin
{
FontLoader::FontLoader()
{
    m_platformLoader = createFontLoader();
    if (!m_platformLoader)
    {
        throw std::runtime_error("Failed to create platform font loader");
    }
}

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