#ifndef KARIN_GRAPHICS_TEXT_BLOB_H
#define KARIN_GRAPHICS_TEXT_BLOB_H

#include <karin/common/geometry/rectangle.h>
#include "font_face.h"
#include "font.h"

#include <vector>
#include <memory>

namespace karin
{

struct GlyphPosition
{
    // position in layout. pixels
    // bottom-left of the glyph bounding box
    Point position;

    uint32_t glyphIndex;
};

struct TextBlob
{
    std::vector<GlyphPosition> glyphs;
    std::unique_ptr<IFontFace> fontFace;
    Font font;
    float fontEmSize;

    // actual layout size. may be smaller than specified layout size
    Size layoutSize;
};

}

#endif //KARIN_GRAPHICS_TEXT_BLOB_H