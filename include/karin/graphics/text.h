#ifndef KARIN_GRAPHICS_TEXT_H
#define KARIN_GRAPHICS_TEXT_H

#include "font.h"

#include <cstdint>
#include <string>

namespace karin
{
struct Text
{
    enum class HorizontalAlignment : uint8_t
    {
        LEADING,
        TRAILING,
        CENTER,
        JUSTIFIED,
    };

    enum class VerticalAlignment : uint8_t
    {
        TOP,
        CENTER,
        BOTTOM,
    };

    enum class Trimming : uint8_t
    {
        NONE,
        WORD,
        CHARACTER,
    };

    enum class Wrapping : uint8_t
    {
        NONE,
        WORD,
        CHARACTER,
    };

    enum class Direction : uint8_t
    {
        TOP_TO_BOTTOM,
        BOTTOM_TO_TOP,
        LEFT_TO_RIGHT,
        RIGHT_TO_LEFT,
    };

    HorizontalAlignment horizontalAlignment = HorizontalAlignment::LEADING;
    VerticalAlignment verticalAlignment = VerticalAlignment::TOP;
    float lineSpacing = 0.0f;
    Trimming trimming = Trimming::NONE;
    Wrapping wrapping = Wrapping::NONE;
    Direction flowDirection = Direction::TOP_TO_BOTTOM;
    Direction readingDirection = Direction::LEFT_TO_RIGHT;

    Font font;

    std::string text;
};
} // karin

#endif //KARIN_GRAPHICS_TEXT_H
