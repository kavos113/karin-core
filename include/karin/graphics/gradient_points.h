#ifndef KARIN_GRAPHICS_GRADIENT_POINT_H
#define KARIN_GRAPHICS_GRADIENT_POINT_H
#include <karin/common/color/color.h>

#include <vector>

namespace karin
{
struct GradientPoints
{
    struct GradientPoint
    {
        float offset;
        Color color;
    };

    enum class ExtendMode
    {
        CLAMP = 0,
        REPEAT = 1,
        MIRROR = 2
    };

    std::vector<GradientPoint> points;
    ExtendMode extendMode = ExtendMode::CLAMP;

    size_t hash() const;
};
} // karin

#endif //KARIN_GRAPHICS_GRADIENT_POINT_H
