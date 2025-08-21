#ifndef KARIN_GRAPHICS_FONT_H
#define KARIN_GRAPHICS_FONT_H

#include <string>
#include <cstdint>

namespace karin
{
struct TextFormat
{
    enum class Style : uint8_t
    {
        NORMAL,
        ITALIC,
        OBLIQUE,
    };

    enum class Stretch : uint8_t
    {
        ULTRA_CONDENSED,
        EXTRA_CONDENSED,
        CONDENSED,
        SEMI_CONDENSED,
        NORMAL,
        SEMI_EXPANDED,
        EXPANDED,
        EXTRA_EXPANDED,
        ULTRA_EXPANDED,
    };

    enum class Weight : uint16_t
    {
        THIN = 100,
        EXTRA_LIGHT = 200,
        LIGHT = 300,
        SEMI_LIGHT = 350,
        NORMAL = 400,
        MEDIUM = 500,
        SEMI_BOLD = 600,
        BOLD = 700,
        EXTRA_BOLD = 800,
        BLACK = 900,
        EXTRA_BLACK = 950,
    };

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

    float size = 0.0f;
    std::string family;
    Style style = Style::NORMAL;
    Stretch stretch = Stretch::NORMAL;
    Weight weight = Weight::NORMAL;

    HorizontalAlignment horizontalAlignment = HorizontalAlignment::LEADING;
    VerticalAlignment verticalAlignment = VerticalAlignment::TOP;
    float lineSpacing = 0.0f;
    Trimming trimming = Trimming::NONE;
    Wrapping wrapping = Wrapping::NONE;
    Direction flowDirection = Direction::TOP_TO_BOTTOM;
    Direction readingDirection = Direction::LEFT_TO_RIGHT;

    std::string locale = "en-US";

    size_t hash() const;
};
} // karin

#endif //KARIN_GRAPHICS_FONT_H
