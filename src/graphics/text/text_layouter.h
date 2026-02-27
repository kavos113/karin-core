#ifndef KARIN_FONT_LAYOUTER_H
#define KARIN_FONT_LAYOUTER_H

#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/text_layout.h>
#include <karin/graphics/font_face.h>

#include <cstdint>
#include <vector>

namespace karin
{
class TextLayouter
{
public:
    TextLayouter() = default;
    ~TextLayouter() = default;

    struct GlyphPosition
    {
        // position in layout. pixels
        // origin: top-left
        Rectangle position;

        uint32_t glyphIndex;
    };

    static std::vector<GlyphPosition> layout(const TextLayout& layout, IFontFace *face);
};
} // karin

#endif //KARIN_FONT_LAYOUTER_H