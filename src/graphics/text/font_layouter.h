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
    FontLayouter(FontLoader* fontLoader)
        : m_fontLoader(fontLoader)
    {
    }
    ~FontLayouter() = default;

    struct GlyphPosition
    {
        // position in layout. pixels
        Rectangle position;

        uint32_t glyphIndex;
    };

    std::vector<GlyphPosition> layout(const TextLayout& layout, FT_Face face) const;

private:
    FontLoader* m_fontLoader = nullptr;
};
} // karin

#endif //KARIN_FONT_LAYOUTER_H