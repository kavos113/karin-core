#ifndef KARIN_GRAPHICS_TEXT_H
#define KARIN_GRAPHICS_TEXT_H

#include "text_format.h"

#include <karin/common/geometry/size.h>

#include <string>

namespace karin
{
struct TextLayout
{
    Size size = {0.0f, 0.0f};

    bool underline = false;
    bool lineThrough = false;

    TextFormat format;
    std::string text;

    size_t hash() const;
};
} // karin

#endif //KARIN_GRAPHICS_TEXT_H
