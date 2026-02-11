#include "freetype_font_face.h"

#include <hb-ft.h>

namespace karin
{
FreetypeFontFace::FreetypeFontFace(FT_Face face)
{
    m_fbFont = hb_ft_font_create(face, nullptr);
    m_face = face;
}

FreetypeFontFace::~FreetypeFontFace()
{
    if (m_fbFont)
    {
        hb_font_destroy(m_fbFont);
    }
}

hb_font_t* FreetypeFontFace::getHbFont()
{
    return m_fbFont;
}

FT_Face FreetypeFontFace::face()
{
    return m_face;
}

FontMetrics FreetypeFontFace::getFontMetrics() const
{
    return FontMetrics{
        .ascender = static_cast<float>(m_face->size->metrics.ascender) / 64.0f,
        .descender = static_cast<float>(m_face->size->metrics.descender) / 64.0f,
        .height = static_cast<float>(m_face->size->metrics.height) / 64.0f,
        .maxAdvance = static_cast<float>(m_face->size->metrics.max_advance) / 64.0f,
    };
}
} // karin