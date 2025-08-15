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
    hash_combine(seed, static_cast<int>(extendMode));

    for (const auto& point : gradientPoints)
    {
        hash_combine(seed, point.offset);
        hash_combine(seed, point.color.r);
        hash_combine(seed, point.color.g);
        hash_combine(seed, point.color.b);
        hash_combine(seed, point.color.a);
    }

    return seed;
}

size_t LinearGradientPattern::pointsHash() const
{
    size_t seed = 0;

    hash_combine(seed, static_cast<int>(extendMode));

    for (const auto& point : gradientPoints)
    {
        hash_combine(seed, point.offset);
        hash_combine(seed, point.color.r);
        hash_combine(seed, point.color.g);
        hash_combine(seed, point.color.b);
        hash_combine(seed, point.color.a);
    }

    return seed;
}
}
