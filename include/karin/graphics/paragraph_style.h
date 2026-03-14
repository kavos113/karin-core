#ifndef KARIN_GRAPHICS_PARAGRAPH_STYLE_H
#define KARIN_GRAPHICS_PARAGRAPH_STYLE_H

#include <cstdint>

namespace karin
{
struct ParagraphStyle
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

    enum class LineSpacingMode : uint8_t
    {
        DEFAULT,
        UNIFORM,
        PROPORTIONAL,
    };

    HorizontalAlignment horizontalAlignment = HorizontalAlignment::LEADING;
    VerticalAlignment verticalAlignment = VerticalAlignment::TOP;

    float lineSpacing = 0.0f;
    float baseline = 0.0f;
    LineSpacingMode lineSpacingMode = LineSpacingMode::DEFAULT;

    Trimming trimming = Trimming::NONE;
    Wrapping wrapping = Wrapping::NONE;

    Direction flowDirection = Direction::TOP_TO_BOTTOM;
    Direction readingDirection = Direction::LEFT_TO_RIGHT;

    size_t hash() const;
};
}

#endif //KARIN_GRAPHICS_PARAGRAPH_STYLE_H