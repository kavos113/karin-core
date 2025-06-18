#ifndef KARIN_GRAPHICS_GRAPHICS_STROKE_STYLE_H
#define KARIN_GRAPHICS_GRAPHICS_STROKE_STYLE_H
#include <vector>
#include <tuple>

namespace karin
{
struct StrokeStyle
{
    enum class CapStyle
    {
        Butt,
        Round,
        Square,
        Triangle,
    };

    enum class JoinStyle
    {
        Miter,
        Round,
        Bevel
    };

    float width = 1.0f;
    CapStyle start_cap_style = CapStyle::Butt;
    CapStyle end_cap_style = CapStyle::Butt;
    CapStyle dash_cap_style = CapStyle::Butt;
    JoinStyle join_style = JoinStyle::Miter;
    float miter_limit = 10.0f; // Only used if join_style is Miter

    std::vector<float> dash_pattern;
    float dash_offset = 0.0f;

    bool operator< (const StrokeStyle& other) const
    {
        return std::tie(start_cap_style, end_cap_style, dash_cap_style, join_style, miter_limit, dash_pattern, dash_offset) <
               std::tie(other.start_cap_style, other.end_cap_style, other.dash_cap_style, other.join_style, other.miter_limit, other.dash_pattern, other.dash_offset);
    }
};
}

#endif //KARIN_GRAPHICS_GRAPHICS_STROKE_STYLE_H
