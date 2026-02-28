#include "dwrite_font_face.h"

#include <hb-directwrite.h>
#include <iostream>
#include <functional>

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

hb_font_t* DwriteFontFace::getHbFont() const
{
    return m_hbFont;
}

FontMetrics DwriteFontFace::getFontMetrics() const
{
    DWRITE_FONT_METRICS metrics{};
    m_face->GetMetrics(&metrics);

    const void *os2Table = nullptr;
    UINT32 os2TableSize = 0;
    void *context = nullptr;
    BOOL exists = FALSE;
    HRESULT hr = m_face->TryGetFontTable(DWRITE_MAKE_OPENTYPE_TAG('O', 'S', '/', '2'), &os2Table, &os2TableSize, &context, &exists);
    if (FAILED(hr) || !exists)
    {
        std::cerr << "Failed to get OS/2 table." << std::endl;
    }
    else
    {
        if (os2TableSize >= 74)
        {
            // read as big-endian
            std::function readUInt16 = [os2Table](size_t offset) {
                return static_cast<int16_t>(
                    static_cast<const uint8_t *>(os2Table)[offset] << 8 |
                    static_cast<const uint8_t *>(os2Table)[offset + 1]);
            };

            metrics.ascent = readUInt16(68); // sTypoAscender
            metrics.descent = std::abs(readUInt16(70)); // sTypoDescender, positive value
            metrics.lineGap = readUInt16(72); // sTypoLineGap
        }

        m_face->ReleaseFontTable(context);
    }

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

// unit: design units. need to be divided by unitsPerEm to get em unit
GlyphMetrics DwriteFontFace::getGlyphMetrics(uint32_t glyphIndex) const
{
    UINT16 glyphIndex16 = static_cast<UINT16>(glyphIndex);

    DWRITE_GLYPH_METRICS metrics{};
    HRESULT hr = m_face->GetDesignGlyphMetrics(&glyphIndex16, 1, &metrics);
    if (FAILED(hr))
    {
        std::cerr << "Failed to get glyph metrics for glyph index " << glyphIndex << std::endl;
        return GlyphMetrics{glyphIndex, 0, 0, 0, 0, 0};
    }

    std::cout << "Glyph index: " << glyphIndex << ", advanceWidth: " << metrics.advanceWidth
              << ", advanceHeight: " << metrics.advanceHeight
              << ", leftSideBearing: " << metrics.leftSideBearing
              << ", topSideBearing: " << metrics.topSideBearing
              << std::endl;

    return GlyphMetrics{
        glyphIndex,
        static_cast<float>(metrics.advanceWidth),
        static_cast<float>(metrics.advanceHeight),
        static_cast<float>(metrics.leftSideBearing),
        static_cast<float>(metrics.advanceHeight - metrics.topSideBearing),
        static_cast<float>(metrics.advanceWidth) // advanceX is the same as advanceWidth in horizontal layout
    };
}

Microsoft::WRL::ComPtr<IDWriteFontFace> DwriteFontFace::face()
{
    return m_face;
}
} // karin