#ifndef KARIN_GRAPHICS_GRAPHICS_STROKE_STYLE_H
#define KARIN_GRAPHICS_GRAPHICS_STROKE_STYLE_H
#include <vector>

struct StrokeStyle
{
    enum class CapStyle
    {
        Butt,
        Round,
        Square
    };

    enum class JoinStyle
    {
        Miter,
        Round,
        Bevel
    };

    float width = 1.0f;
    CapStyle cap_style = CapStyle::Butt;
    CapStyle dash_cap_style = CapStyle::Butt;
    JoinStyle join_style = JoinStyle::Miter;
    float miter_limit = 10.0f; // Only used if join_style is Miter

    std::vector<float> dash_pattern;
    float dash_offset = 0.0f;
};

#endif //KARIN_GRAPHICS_GRAPHICS_STROKE_STYLE_H
