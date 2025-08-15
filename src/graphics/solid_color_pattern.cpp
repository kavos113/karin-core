#include <karin/graphics/solid_color_pattern.h>

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
}
