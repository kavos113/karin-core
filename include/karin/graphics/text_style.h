#ifndef KARIN_GRAPHICS_TEXT_STYLE_H
#define KARIN_GRAPHICS_TEXT_STYLE_H

#include "font.h"

#include <string>

namespace karin
{
struct TextStyle
{
    Font font;
    float size = 0.0f;
    std::string locale = "en-US";

    bool underline = false;
    bool lineThrough = false;

    size_t hash() const;
};
} // karin

#endif //KARIN_GRAPHICS_TEXT_STYLE_H