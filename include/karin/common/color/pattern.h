#ifndef KARIN_COMMON_COLOR_PATTERN_H
#define KARIN_COMMON_COLOR_PATTERN_H

#include "solid_color_pattern.h"
#include "linear_gradient_pattern.h"

#include <variant>

namespace karin
{
using Pattern = std::variant<SolidColorPattern, LinearGradientPattern>;
}

#endif //KARIN_COMMON_COLOR_PATTERN_H
