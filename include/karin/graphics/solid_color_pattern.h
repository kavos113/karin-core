#ifndef KARIN_COMMON_COLOR_SOLID_COLOR_PATTERN_H
#define KARIN_COMMON_COLOR_SOLID_COLOR_PATTERN_H

#include <karin/common/color/color.h>

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
} // karin

#endif //KARIN_COMMON_COLOR_SOLID_COLOR_PATTERN_H
