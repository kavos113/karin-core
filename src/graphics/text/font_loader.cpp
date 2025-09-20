#include "font_loader.h"

#include <stdexcept>

namespace karin
{
FontLoader::FontLoader(SystemFont* systemFont)
    : m_systemFont(systemFont)
{
    FT_Error error = FT_Init_FreeType(&m_library);
    if (error)
    {
        throw std::runtime_error("Failed to initialize FreeType library");
    }
}

FontLoader::~FontLoader()
{
    FT_Done_FreeType(m_library);
}

FT_Face FontLoader::loadFont(const Font& font)
{
    if (auto it = m_faces.find(font.hash()); it != m_faces.end())
    {
        return it->second;
    }

    FT_Face face = nullptr;
    auto fontData = m_systemFont->getFontData(font);
    if (fontData.empty())
    {
        throw std::runtime_error("Font not found: " + font.family);
    }

    m_fontDataStorage.push_back(std::move(fontData));

    FT_Error error = FT_New_Memory_Face(
        m_library,
        reinterpret_cast<const FT_Byte*>(m_fontDataStorage.back().data()),
        static_cast<FT_Long>(m_fontDataStorage.back().size()),
        0,
        &face
    );
    if (error)
    {
        throw std::runtime_error("Failed to create font face from memory");
    }

    m_faces[font.hash()] = face;
    return face;
}
} // karin
