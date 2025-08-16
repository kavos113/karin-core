#ifndef KARIN_COMMON_COLOR_LINEAR_GRADIENT_PATTERN_H
#define KARIN_COMMON_COLOR_LINEAR_GRADIENT_PATTERN_H

#include "gradient_points.h"

#include <karin/common/geometry/point.h>

namespace karin
{
struct LinearGradientPattern
{
    Point start;
    Point end;
    GradientPoints gradientPoints;

    size_t hash() const;
    size_t pointsHash() const;
};
} // karin

#endif //KARIN_COMMON_COLOR_LINEAR_GRADIENT_PATTERN_H
