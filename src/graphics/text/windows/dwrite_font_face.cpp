#include "dwrite_font_face.h"

#include <hb-directwrite.h>

namespace karin
{
DwriteFontFace::DwriteFontFace(const Microsoft::WRL::ComPtr<IDWriteFontFace>& face)
{
    m_hbFont = hb_directwrite_font_create(face.Get());
    m_face = face;
}

DwriteFontFace::~DwriteFontFace()
{
    if (m_hbFont)
    {
        hb_font_destroy(m_hbFont);
    }
}

hb_font_t* DwriteFontFace::getHbFont()
{
    return m_hbFont;
}

FontMetrics DwriteFontFace::getFontMetrics() const
{
    DWRITE_FONT_METRICS metrics{};
    m_face->GetMetrics(&metrics);

    return FontMetrics{
        metrics.designUnitsPerEm,
        metrics.ascent,
        metrics.descent,
        metrics.lineGap,
        metrics.capHeight,
        metrics.xHeight,
        metrics.underlinePosition,
        metrics.underlineThickness,
        metrics.strikethroughPosition,
        metrics.strikethroughThickness
    };
}

GlyphMetrics DwriteFontFace::getGlyphMetrics(uint32_t glyphIndex) const
{
    return {};
}

Microsoft::WRL::ComPtr<IDWriteFontFace> DwriteFontFace::face()
{
    return m_face;
}
} // karin