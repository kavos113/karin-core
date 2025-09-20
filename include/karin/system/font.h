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

    std::string family;
    Style style = Style::NORMAL;
    Stretch stretch = Stretch::NORMAL;
    Weight weight = Weight::NORMAL;

    size_t hash() const;
};
} // karin

#endif //KARIN_GRAPHICS_FONT_H
