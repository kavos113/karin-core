#ifndef KARIN_GRAPHICS_TEXT_LAYOUT_H
#define KARIN_GRAPHICS_TEXT_LAYOUT_H

#include "text_format.h"

#include <karin/common/geometry/size.h>

#include <string>
#include <utility>

namespace karin
{
// size, text is required
struct TextLayout
{
    Size size = {0.0f, 0.0f};

    TextFormat format;
    std::string text;

    bool underline = false;
    bool lineThrough = false;

    size_t hash() const;

    TextLayout(
        std::string text,
        float fontSize,
        std::string fontFamily,
        Size textRegion
    ) : size(textRegion),
        text(std::move(text))
    {
        format.size = fontSize;
        format.font.family = std::move(fontFamily);
    }

    TextLayout(
        std::string text,
        const TextFormat& textFormat,
        Size textRegion,
        bool underline = false,
        bool lineThrough = false
    ) : size(textRegion),
        format(textFormat),
        text(std::move(text)),
        underline(underline),
        lineThrough(lineThrough)
    {}
};
} // karin

#endif //KARIN_GRAPHICS_TEXT_LAYOUT_H