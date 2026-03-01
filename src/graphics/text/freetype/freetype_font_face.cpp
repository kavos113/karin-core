#include "freetype_font_face.h"

#include <hb-ft.h>
#include <hb-ot.h>
#include FT_TRUETYPE_TABLES_H

#include <cmath>
#include <iostream>

namespace karin
{
FreetypeFontFace::FreetypeFontFace(FT_Face face)
{
    hb_face_t* hbFace = hb_ft_face_create(face, nullptr);
    m_fbFont = hb_font_create(hbFace);
    hb_ot_font_set_funcs(m_fbFont);
    hb_font_set_scale(m_fbFont, face->units_per_EM, face->units_per_EM);

    m_face = face;
}

FreetypeFontFace::~FreetypeFontFace()
{
    if (m_fbFont)
    {
        hb_font_destroy(m_fbFont);
    }
}

hb_font_t* FreetypeFontFace::getHbFont() const
{
    return m_fbFont;
}

FT_Face FreetypeFontFace::face()
{
    return m_face;
}

FontMetrics FreetypeFontFace::getFontMetrics() const
{
    FontMetrics metrics{
        .unitsPerEm = m_face->units_per_EM,
    };

    auto *os2 = static_cast<TT_OS2 *>(FT_Get_Sfnt_Table(m_face, ft_sfnt_os2));
    auto *hhea = static_cast<TT_HoriHeader *>(FT_Get_Sfnt_Table(m_face, ft_sfnt_hhea));

    if (os2)
    {
        metrics.ascender = os2->sTypoAscender;
        metrics.descender = std::abs(os2->sTypoDescender);
        metrics.lineGap = os2->sTypoLineGap;
    }
    else if (hhea)
    {
        metrics.ascender = hhea->Ascender;
        metrics.descender = std::abs(hhea->Descender);
        metrics.lineGap = hhea->Line_Gap;
    }
    else
    {
        metrics.ascender = m_face->ascender;
        metrics.descender = std::abs(m_face->descender);
        metrics.lineGap = m_face->height - (m_face->ascender - m_face->descender);
    }

    if (os2 && os2->version >= 2)
    {
        metrics.capHeight = os2->sCapHeight;
        metrics.xHeight = os2->sxHeight;
    }
    else
    {
        FT_Set_Pixel_Sizes(m_face, 0, metrics.unitsPerEm);

        uint32_t index = FT_Get_Char_Index(m_face, 'H');
        if (index)
        {
            FT_Load_Glyph(m_face, index, FT_LOAD_NO_SCALE);
            metrics.capHeight = m_face->glyph->metrics.horiBearingY;
        }
        index = FT_Get_Char_Index(m_face, 'x');
        if (index)
        {
            FT_Load_Glyph(m_face, index, FT_LOAD_NO_SCALE);
            metrics.xHeight = m_face->glyph->metrics.horiBearingY;
        }
    }

    metrics.underlineThickness = m_face->underline_thickness;
    // dwrite: top of underline, freetype: center of underline
    metrics.underlinePosition = m_face->underline_position + metrics.underlineThickness / 2;

    if (os2)
    {
        metrics.strikethroughPosition = os2->yStrikeoutPosition;
        metrics.strikethroughThickness = os2->yStrikeoutSize;
    }
    else
    {
        uint32_t index = FT_Get_Char_Index(m_face, '0');
        if (index)
        {
            FT_Load_Glyph(m_face, index, FT_LOAD_NO_SCALE);
            metrics.strikethroughPosition = m_face->glyph->metrics.horiBearingY / 2;
            metrics.strikethroughThickness = metrics.underlineThickness;
        }
    }

    return metrics;
}

GlyphMetrics FreetypeFontFace::getGlyphMetrics(uint32_t glyphIndex) const
{
    FT_Error error = FT_Load_Glyph(m_face, glyphIndex, FT_LOAD_NO_BITMAP | FT_LOAD_NO_SCALE);
    if (error)
    {
        std::cerr << "Failed to load glyph index " << glyphIndex << ": " << error << std::endl;
        return GlyphMetrics{glyphIndex, 0, 0, 0, 0, 0};
    }

    error = FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
    if (error)
    {
        std::cerr << "Failed to render glyph index " << glyphIndex << ": " << error << std::endl;
        return GlyphMetrics{glyphIndex, 0, 0, 0, 0, 0};
    }

    FT_Glyph_Metrics metrics = m_face->glyph->metrics;
    return GlyphMetrics{
        .glyphIndex = glyphIndex,
        .width = static_cast<float>(metrics.width),
        .height = static_cast<float>(metrics.height),
        .bearingX = static_cast<float>(metrics.horiBearingX),
        .bearingY = static_cast<float>(metrics.horiBearingY),
        .advanceX = static_cast<float>(metrics.horiAdvance)
    };
}
} // karin