#ifndef KARIN_GRAPHICS_RADICAL_GRADIENT_PATTERN_H
#define KARIN_GRAPHICS_RADICAL_GRADIENT_PATTERN_H
#include "gradient_points.h"

#include <karin/common/geometry/point.h>


namespace karin
{
struct RadicalGradientPattern
{
    Point center;
    Point offset;
    float radiusX;
    float radiusY;
    GradientPoints gradientPoints;

    size_t hash() const;
};
}


#endif //KARIN_GRAPHICS_RADICAL_GRADIENT_PATTERN_H
