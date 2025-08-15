#ifndef KARIN_COMMON_COLOR_LINEAR_GRADIENT_PATTERN_H
#define KARIN_COMMON_COLOR_LINEAR_GRADIENT_PATTERN_H

#include "../common/color/color.h"

#include <karin/common/geometry/point.h>

#include <vector>

namespace karin
{
struct LinearGradientPattern
{
    struct GradientPoint
    {
        float offset;
        Color color;

        bool operator<(const GradientPoint& other) const
        {
            return offset < other.offset || (offset == other.offset && color < other.color);
        }

        bool operator==(const GradientPoint& other) const
        {
            return offset == other.offset && color == other.color;
        }
    };

    enum class ExtendMode
    {
        CLAMP,
        REPEAT,
        MIRROR
    };

    Point start;
    Point end;
    std::vector<GradientPoint> gradientPoints;
    ExtendMode extendMode = ExtendMode::CLAMP;

    size_t hash() const;
};
} // karin

#endif //KARIN_COMMON_COLOR_LINEAR_GRADIENT_PATTERN_H