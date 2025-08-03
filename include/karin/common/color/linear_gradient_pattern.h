#ifndef KARIN_COMMON_COLOR_LINEAR_GRADIENT_PATTERN_H
#define KARIN_COMMON_COLOR_LINEAR_GRADIENT_PATTERN_H

#include "color.h"

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

    bool operator<(const LinearGradientPattern& other) const
    {
        if (start != other.start)
            return start.x < other.start.x || (start.x == other.start.x && start.y < other.start.y);
        if (end != other.end)
            return end.x < other.end.x || (end.x == other.end.x && end.y < other.end.y);
        if (gradientPoints != other.gradientPoints)
            return gradientPoints < other.gradientPoints;
        return extendMode < other.extendMode;
    }
};
} // karin

#endif //KARIN_COMMON_COLOR_LINEAR_GRADIENT_PATTERN_H
