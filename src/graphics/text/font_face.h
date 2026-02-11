#ifndef SRC_GRAPHICS_TEXT_FONT_FACE_H
#define SRC_GRAPHICS_TEXT_FONT_FACE_H

#include <hb.h>

namespace karin
{

// real value (not fractional)
struct FontMetrics
{
    float ascender;
    float descender;
    float height;
    float maxAdvance;
};

class IFontFace
{
public:
    virtual ~IFontFace() = 0;

    virtual hb_font_t *getHbFont() = 0;
    virtual FontMetrics getFontMetrics() const = 0;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FONT_FACE_H