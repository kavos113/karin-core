#ifndef KARIN_COMMON_COLOR_PATTERN_H
#define KARIN_COMMON_COLOR_PATTERN_H

#include "image.h"

#include <karin/common/color/color.h>
#include <karin/common/geometry/point.h>
#include <variant>
#include <vector>

namespace karin
{
class SolidColorPattern
{
public:
    explicit SolidColorPattern(const Color& color) : m_color(color)
    {
    }

    [[nodiscard]] const Color& color() const
    {
        return m_color;
    }

    size_t hash() const;

private:
    Color m_color;
};

enum class ExtendMode
{
    CLAMP = 0,
    REPEAT = 1,
    MIRROR = 2
};

struct GradientPoints
{
    struct GradientPoint
    {
        float offset;
        Color color;
    };

    std::vector<GradientPoint> points;
    ExtendMode extendMode = ExtendMode::CLAMP;

    size_t hash() const;
};

struct LinearGradientPattern
{
    Point start;
    Point end;
    GradientPoints gradientPoints;

    size_t hash() const;
};

struct RadialGradientPattern
{
    Point center;
    Point offset;
    float radiusX;
    float radiusY;
    GradientPoints gradientPoints;

    size_t hash() const;
};

struct ImagePattern
{
    Image image;
    Point offset;
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    ExtendMode extendModeX = ExtendMode::CLAMP;
    ExtendMode extendModeY = ExtendMode::CLAMP;

    size_t hash() const;
};

using Pattern = std::variant<SolidColorPattern, LinearGradientPattern, RadialGradientPattern, ImagePattern>;

enum class PatternType : uint32_t
{
    SolidColor = 0,
    LinearGradient = 1,
    RadialGradient = 2,
    Image = 3
};
}

#endif //KARIN_COMMON_COLOR_PATTERN_H
