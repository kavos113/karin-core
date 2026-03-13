#ifndef KARIN_FONT_LAYOUTER_H
#define KARIN_FONT_LAYOUTER_H

#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/text_layout.h>
#include <karin/graphics/font_face.h>
#include <karin/graphics/text_blob.h>

#include <cstdint>
#include <vector>

namespace karin
{
class TextLayouter
{
public:
    TextLayouter() = default;
    ~TextLayouter() = default;

    static std::vector<GlyphPosition> layout(const TextLayout& layout, IFontFace *face, Size& outLayoutSize);
};
} // karin

#endif //KARIN_FONT_LAYOUTER_H