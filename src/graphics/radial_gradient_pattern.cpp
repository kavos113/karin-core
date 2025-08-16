#include <karin/graphics/radial_gradient_pattern.h>
#include <utils/hash.h>

namespace karin
{
size_t RadialGradientPattern::hash() const
{
    size_t seed = 0;

    hash_combine(seed, center.x);
    hash_combine(seed, center.y);
    hash_combine(seed, offset.x);
    hash_combine(seed, offset.y);
    hash_combine(seed, radiusX);
    hash_combine(seed, radiusY);
    hash_combine(seed, gradientPoints.hash());

    return seed;
}
}
