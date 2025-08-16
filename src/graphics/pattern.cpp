#include <karin/graphics/pattern.h>
#include <utils/hash.h>

namespace karin
{
size_t SolidColorPattern::hash() const
{
    size_t seed = 0;

    hash_combine(seed, m_color.r);
    hash_combine(seed, m_color.g);
    hash_combine(seed, m_color.b);
    hash_combine(seed, m_color.a);

    return seed;
}

size_t GradientPoints::hash() const
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

size_t ImagePattern::hash() const
{
    size_t seed = 0;

    hash_combine(seed, image.hash());
    hash_combine(seed, offset.x);
    hash_combine(seed, offset.y);
    hash_combine(seed, scaleX);
    hash_combine(seed, scaleY);
    hash_combine(seed, static_cast<int>(extendModeX));
    hash_combine(seed, static_cast<int>(extendModeY));

    return seed;
}
}
