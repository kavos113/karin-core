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

FontLoader::FontMetrics FontLoader::glyphMetrics(const Font &font, uint32_t size, uint32_t glyphIndex)
{
    if (auto it = m_glyphMetricsCache.find(Glyph{font, glyphIndex}.hash()); it != m_glyphMetricsCache.end())
    {
        return it->second;
    }

    FT_Face face = loadFont(font);
    FT_Error error = FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(size));
    if (error)
    {
        throw std::runtime_error("failed to set pixel sizes for font");
    }

    error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_NO_BITMAP);
    if (error)
    {
        throw std::runtime_error("failed to load glyph");
    }

    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    if (error)
    {
        throw std::runtime_error("failed to render glyph");
    }

    FT_GlyphSlot slot = face->glyph;
    FontMetrics metrics = {
        .width = static_cast<float>(slot->bitmap.width),
        .height = static_cast<float>(slot->bitmap.rows),
        .bearingX = static_cast<float>(slot->bitmap_left),
        .bearingY = static_cast<float>(slot->bitmap_top),
        .advanceX = static_cast<float>(slot->advance.x) / 64.0f,
    };
    m_glyphMetricsCache[Glyph{font, glyphIndex}.hash()] = metrics;
    return metrics;
}
} // karin