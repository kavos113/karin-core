#ifndef SRC_GRAPHICS_VULKAN_SHADERS_LINEAR_GRADIENT_PUSH_CONSTANTS_H
#define SRC_GRAPHICS_VULKAN_SHADERS_LINEAR_GRADIENT_PUSH_CONSTANTS_H

#ifdef __cplusplus
#include <karin/graphics/linear_gradient_pattern.h>
#include <glm/glm.hpp>

namespace karin
{
struct LinearGradientPushConstants
{
    glm::vec2 start;
    glm::vec2 end;
    glm::vec3 shapeParams;
    ShapeType shapeType = ShapeType::Nothing;
    LinearGradientPattern::ExtendMode extendMode = LinearGradientPattern::ExtendMode::CLAMP;
};
}

#else // glsl

layout (push_constant) uniform LinearGradientPushConstants
{
    vec4 coords;
    vec3 shapeParams;
    uint shapeType;
    uint extendMode; // LinearGradientPattern::ExtendMode
} linearGradientPushConstants;

#endif

#endif //SRC_GRAPHICS_VULKAN_SHADERS_LINEAR_GRADIENT_PUSH_CONSTANTS_H
