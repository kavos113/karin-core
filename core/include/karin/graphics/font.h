#ifndef KARIN_GRAPHICS_FONT_H
#define KARIN_GRAPHICS_FONT_H

#include <string>
#include <cstdint>

namespace karin
{
struct Font
{
    enum class Style : uint8_t
    {
        NORMAL = 0,
        ITALIC = 1,
        OBLIQUE = 2,
    };

    enum class Stretch : uint8_t
    {
        ULTRA_CONDENSED = 0,
        EXTRA_CONDENSED = 1,
        CONDENSED = 2,
        SEMI_CONDENSED = 3,
        NORMAL = 4,
        SEMI_EXPANDED = 5,
        EXPANDED = 6,
        EXTRA_EXPANDED = 7,
        ULTRA_EXPANDED = 8,
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

    std::string family;
    Style style = Style::NORMAL;
    Stretch stretch = Stretch::NORMAL;
    Weight weight = Weight::NORMAL;

    size_t hash() const;
};
} // karin

#endif //KARIN_GRAPHICS_FONT_H
