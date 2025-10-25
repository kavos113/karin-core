#ifndef KARIN_FONT_LAYOUTER_H
#define KARIN_FONT_LAYOUTER_H

#include "font_loader.h"

#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/text_layout.h>

#include <cstdint>
#include <memory>
#include <vector>

namespace karin
{
class FontLayouter
{
public:
    struct GlyphPosition
    {
        // position in layout. pixels
        Rectangle position;

        uint32_t glyphIndex;
    };

    std::vector<GlyphPosition> layout(const TextLayout& layout) const;

private:
    std::unique_ptr<FontLoader> m_fontLoader;
};
} // karin

#endif //KARIN_FONT_LAYOUTER_H