#include <karin/graphics/linear_gradient_pattern.h>

#include <utils/hash.h>

namespace karin
{
size_t LinearGradientPattern::hash() const
{
    size_t seed = 0;

    hash_combine(seed, start.x);
    hash_combine(seed, start.y);
    hash_combine(seed, end.x);
    hash_combine(seed, end.y);
    hash_combine(seed, gradientPoints.hash());

    return seed;
}
}
