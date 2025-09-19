#include <karin/graphics/pattern.h>
#include <karin/graphics/text_format.h>
#include <karin/graphics/text_layout.h>
#include <karin/system/font.h>

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

size_t Font::hash() const
{
    size_t seed = 0;

    hash_combine(seed, family);
    hash_combine(seed, static_cast<int>(style));
    hash_combine(seed, static_cast<int>(stretch));
    hash_combine(seed, static_cast<int>(weight));
    return seed;
}


size_t TextFormat::hash() const
{
    size_t seed = 0;

    hash_combine(seed, size);
    hash_combine(seed, font.hash());
    hash_combine(seed, static_cast<int>(horizontalAlignment));
    hash_combine(seed, static_cast<int>(verticalAlignment));
    hash_combine(seed, lineSpacing);
    hash_combine(seed, baseline);
    hash_combine(seed, static_cast<int>(lineSpacingMode));
    hash_combine(seed, static_cast<int>(trimming));
    hash_combine(seed, static_cast<int>(wrapping));
    hash_combine(seed, static_cast<int>(flowDirection));
    hash_combine(seed, static_cast<int>(readingDirection));
    hash_combine(seed, locale);
    return seed;
}

size_t TextLayout::hash() const
{
    size_t seed = 0;

    hash_combine(seed, underline);
    hash_combine(seed, lineThrough);
    hash_combine(seed, size.width);
    hash_combine(seed, size.height);
    hash_combine(seed, format.hash());
    hash_combine(seed, text);

    return seed;
}
}
