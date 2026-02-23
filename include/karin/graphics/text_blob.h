#ifndef KARIN_GRAPHICS_TEXT_BLOB_H
#define KARIN_GRAPHICS_TEXT_BLOB_H

#include <karin/common/geometry/rectangle.h>
#include "font_face.h"

#include <vector>
#include <memory>

namespace karin
{

struct GlyphPosition
{
    // position in layout. pixels
    // origin: top-left
    Rectangle position;

    uint32_t glyphIndex;
};

struct TextBlob
{
    std::vector<GlyphPosition> glyphs;
    std::unique_ptr<IFontFace> fontFace;
};

}

#endif //KARIN_GRAPHICS_TEXT_BLOB_H