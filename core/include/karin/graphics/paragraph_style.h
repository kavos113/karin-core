#ifndef KARIN_GRAPHICS_PARAGRAPH_STYLE_H
#define KARIN_GRAPHICS_PARAGRAPH_STYLE_H

#include <cstdint>

namespace karin
{
struct ParagraphStyle
{
    enum class HorizontalAlignment : uint8_t
    {
        LEADING = 0,
        TRAILING = 1,
        CENTER = 2,
        JUSTIFIED = 3,
    };

    enum class VerticalAlignment : uint8_t
    {
        TOP = 0,
        CENTER = 1,
        BOTTOM = 2,
    };

    enum class Trimming : uint8_t
    {
        NONE = 0,
        WORD = 1,
        CHARACTER = 2,
    };

    enum class Wrapping : uint8_t
    {
        NONE = 0,
        WORD = 1,
        CHARACTER = 2,
    };

    enum class Direction : uint8_t
    {
        TOP_TO_BOTTOM = 0,
        BOTTOM_TO_TOP = 1,
        LEFT_TO_RIGHT = 2,
        RIGHT_TO_LEFT = 3,
    };

    enum class LineSpacingMode : uint8_t
    {
        DEFAULT = 0,
        UNIFORM = 1,
        PROPORTIONAL = 2,
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