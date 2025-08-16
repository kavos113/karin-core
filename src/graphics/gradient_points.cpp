#include <karin/graphics/gradient_points.h>
#include <utils/hash.h>

size_t karin::GradientPoints::hash() const
{
    size_t seed = 0;

    hash_combine(seed, static_cast<int>(extendMode));
    for (const auto& point : points)
    {
        hash_combine(seed, point.offset);
        hash_combine(seed, point.color.r);
        hash_combine(seed, point.color.g);
        hash_combine(seed, point.color.b);
        hash_combine(seed, point.color.a);
    }

    return seed;
}
