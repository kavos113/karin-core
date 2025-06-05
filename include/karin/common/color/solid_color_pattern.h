#ifndef KARIN_COMMON_COLOR_SOLID_COLOR_PATTERN_H
#define KARIN_COMMON_COLOR_SOLID_COLOR_PATTERN_H

#include "color.h"
#include "pattern.h"

namespace karin {

class SolidColorPattern : public Pattern
{
public:
    explicit SolidColorPattern(const Color& color) : m_color(color) {}

    const Color& color() const { return m_color; }
private:
    Color m_color;
};

} // karin

#endif //KARIN_COMMON_COLOR_SOLID_COLOR_PATTERN_H
