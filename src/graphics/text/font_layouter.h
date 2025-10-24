#ifndef KARIN_FONT_LAYOUTER_H
#define KARIN_FONT_LAYOUTER_H

#include "font_loader.h"

#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/text_layout.h>

namespace karin
{
class FontLayouter
{
public:
    struct GlyphPosition
    {
        // position in layout. pixels
        Rectangle position;

        // uv in atlas. 0.0 - 1.0
        Rectangle uv;
    };

    std::vector<GlyphPosition> layout(const TextLayout& layout);

private:
    std::unique_ptr<FontLoader> m_fontLoader;
};
} // karin

#endif //KARIN_FONT_LAYOUTER_H