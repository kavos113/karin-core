#ifndef KARIN_FONT_LAYOUTER_H
#define KARIN_FONT_LAYOUTER_H

#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/text_style.h>
#include <karin/graphics/paragraph_style.h>
#include <karin/graphics/font_face.h>
#include <karin/graphics/text_blob.h>

#include <vector>
#include <string>

namespace karin
{
class TextLayouter
{
public:
    TextLayouter() = default;
    ~TextLayouter() = default;

    static std::vector<GlyphPosition> layout(
        const IFontFace* face,
        const std::string& text,
        const TextStyle& textStyle,
        const ParagraphStyle& paragraphStyle,
        const Size& maxSize,
        Size& outContentSize
    );
};
} // karin

#endif //KARIN_FONT_LAYOUTER_H