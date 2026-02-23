#ifndef SRC_GRAPHICS_TEXT_FONT_FACE_H
#define SRC_GRAPHICS_TEXT_FONT_FACE_H

#include <hb.h>
#include <cstdint>

namespace karin
{

// same as DWrite format. by unitsPerEm
struct FontMetrics
{
    uint16_t unitsPerEm;
    uint16_t ascender;  // sTypoAscender in OS/2
    uint16_t descender; // sTypoDescender in OS/2, positive value
    int16_t lineGap;
    uint16_t capHeight;
    uint16_t xHeight;
    int16_t underlinePosition;
    uint16_t underlineThickness;
    int16_t strikethroughPosition;
    uint16_t strikethroughThickness;
};

struct GlyphMetrics
{
    uint32_t glyphIndex;
    float width;
    float height;
    float bearingX;
    float bearingY;
    float advanceX;
};

class IFontFace
{
public:
    virtual ~IFontFace() = default;

    virtual hb_font_t *getHbFont() = 0;
    virtual FontMetrics getFontMetrics() const = 0;
    virtual GlyphMetrics getGlyphMetrics(uint32_t glyphIndex) const = 0;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FONT_FACE_H