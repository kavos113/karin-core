#ifndef SRC_COMMON_D2D_DWRITE_CONVERTER_H
#define SRC_COMMON_D2D_DWRITE_CONVERTER_H

#include <karin/system/font.h>

#include <dwrite.h>
#include <stdexcept>

namespace karin
{
inline DWRITE_FONT_WEIGHT toDWriteFontWeight(Font::Weight weight)
{
    switch (weight)
    {
    case Font::Weight::THIN:
        return DWRITE_FONT_WEIGHT_THIN;
    case Font::Weight::EXTRA_LIGHT:
        return DWRITE_FONT_WEIGHT_EXTRA_LIGHT;
    case Font::Weight::LIGHT:
        return DWRITE_FONT_WEIGHT_LIGHT;
    case Font::Weight::SEMI_LIGHT:
        return DWRITE_FONT_WEIGHT_SEMI_LIGHT;
    case Font::Weight::NORMAL:
        return DWRITE_FONT_WEIGHT_NORMAL;
    case Font::Weight::MEDIUM:
        return DWRITE_FONT_WEIGHT_MEDIUM;
    case Font::Weight::SEMI_BOLD:
        return DWRITE_FONT_WEIGHT_SEMI_BOLD;
    case Font::Weight::BOLD:
        return DWRITE_FONT_WEIGHT_BOLD;
    case Font::Weight::EXTRA_BOLD:
        return DWRITE_FONT_WEIGHT_EXTRA_BOLD;
    case Font::Weight::BLACK:
        return DWRITE_FONT_WEIGHT_BLACK;
    case Font::Weight::EXTRA_BLACK:
        return DWRITE_FONT_WEIGHT_EXTRA_BLACK;
    default:
        throw std::invalid_argument("Unknown font weight");
    }
}

inline DWRITE_FONT_STYLE toDWriteFontStyle(Font::Style style)
{
    switch (style)
    {
    case Font::Style::NORMAL:
        return DWRITE_FONT_STYLE_NORMAL;
    case Font::Style::ITALIC:
        return DWRITE_FONT_STYLE_ITALIC;
    case Font::Style::OBLIQUE:
        return DWRITE_FONT_STYLE_OBLIQUE;
    default:
        throw std::invalid_argument("Unknown font style");
    }
}

inline DWRITE_FONT_STRETCH toDWriteFontStretch(Font::Stretch stretch)
{
    switch (stretch)
    {
    case Font::Stretch::ULTRA_CONDENSED:
        return DWRITE_FONT_STRETCH_ULTRA_CONDENSED;
    case Font::Stretch::EXTRA_CONDENSED:
        return DWRITE_FONT_STRETCH_EXTRA_CONDENSED;
    case Font::Stretch::CONDENSED:
        return DWRITE_FONT_STRETCH_CONDENSED;
    case Font::Stretch::SEMI_CONDENSED:
        return DWRITE_FONT_STRETCH_SEMI_CONDENSED;
    case Font::Stretch::NORMAL:
        return DWRITE_FONT_STRETCH_NORMAL;
    case Font::Stretch::SEMI_EXPANDED:
        return DWRITE_FONT_STRETCH_SEMI_EXPANDED;
    case Font::Stretch::EXPANDED:
        return DWRITE_FONT_STRETCH_EXPANDED;
    case Font::Stretch::EXTRA_EXPANDED:
        return DWRITE_FONT_STRETCH_EXTRA_EXPANDED;
    case Font::Stretch::ULTRA_EXPANDED:
        return DWRITE_FONT_STRETCH_ULTRA_EXPANDED;
    default:
        throw std::invalid_argument("Unknown font stretch");
    }
}

inline Font::Weight fromDWriteFontWeight(DWRITE_FONT_WEIGHT weight)
{
    switch (weight)
    {
    case DWRITE_FONT_WEIGHT_THIN:
        return Font::Weight::THIN;
    case DWRITE_FONT_WEIGHT_EXTRA_LIGHT:
        return Font::Weight::EXTRA_LIGHT;
    case DWRITE_FONT_WEIGHT_LIGHT:
        return Font::Weight::LIGHT;
    case DWRITE_FONT_WEIGHT_SEMI_LIGHT:
        return Font::Weight::SEMI_LIGHT;
    case DWRITE_FONT_WEIGHT_NORMAL:
        return Font::Weight::NORMAL;
    case DWRITE_FONT_WEIGHT_MEDIUM:
        return Font::Weight::MEDIUM;
    case DWRITE_FONT_WEIGHT_SEMI_BOLD:
        return Font::Weight::SEMI_BOLD;
    case DWRITE_FONT_WEIGHT_BOLD:
        return Font::Weight::BOLD;
    case DWRITE_FONT_WEIGHT_EXTRA_BOLD:
        return Font::Weight::EXTRA_BOLD;
    case DWRITE_FONT_WEIGHT_BLACK:
        return Font::Weight::BLACK;
    case DWRITE_FONT_WEIGHT_EXTRA_BLACK:
        return Font::Weight::EXTRA_BLACK;
    default:
        throw std::invalid_argument("Unknown DWRITE font weight");
    }
}

inline Font::Style fromDWriteFontStyle(DWRITE_FONT_STYLE style)
{
    switch (style)
    {
    case DWRITE_FONT_STYLE_NORMAL:
        return Font::Style::NORMAL;
    case DWRITE_FONT_STYLE_ITALIC:
        return Font::Style::ITALIC;
    case DWRITE_FONT_STYLE_OBLIQUE:
        return Font::Style::OBLIQUE;
    default:
        throw std::invalid_argument("Unknown DWRITE font style");
    }
}

inline Font::Stretch fromDWriteFontStretch(DWRITE_FONT_STRETCH stretch)
{
    switch (stretch)
    {
    case DWRITE_FONT_STRETCH_ULTRA_CONDENSED:
        return Font::Stretch::ULTRA_CONDENSED;
    case DWRITE_FONT_STRETCH_EXTRA_CONDENSED:
        return Font::Stretch::EXTRA_CONDENSED;
    case DWRITE_FONT_STRETCH_CONDENSED:
        return Font::Stretch::CONDENSED;
    case DWRITE_FONT_STRETCH_SEMI_CONDENSED:
        return Font::Stretch::SEMI_CONDENSED;
    case DWRITE_FONT_STRETCH_NORMAL:
        return Font::Stretch::NORMAL;
    case DWRITE_FONT_STRETCH_SEMI_EXPANDED:
        return Font::Stretch::SEMI_EXPANDED;
    case DWRITE_FONT_STRETCH_EXPANDED:
        return Font::Stretch::EXPANDED;
    case DWRITE_FONT_STRETCH_EXTRA_EXPANDED:
        return Font::Stretch::EXTRA_EXPANDED;
    case DWRITE_FONT_STRETCH_ULTRA_EXPANDED:
        return Font::Stretch::ULTRA_EXPANDED;
    default:
        throw std::invalid_argument("Unknown DWRITE font stretch");
    }
}
}

#endif //SRC_COMMON_D2D_DWRITE_CONVERTER_H