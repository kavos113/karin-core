#ifndef KARIN_COMMON_COLOR_PATTERN_H
#define KARIN_COMMON_COLOR_PATTERN_H

#include "linear_gradient_pattern.h"
#include "radical_gradient_pattern.h"
#include "solid_color_pattern.h"

#include <variant>

namespace karin
{
using Pattern = std::variant<SolidColorPattern, LinearGradientPattern, RadicalGradientPattern>;

enum class PatternType
{
    SolidColor,
    LinearGradient,
    RadicalGradient
};
}

#endif //KARIN_COMMON_COLOR_PATTERN_H
