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
} // karin