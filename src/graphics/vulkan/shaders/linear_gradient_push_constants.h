#ifndef SRC_GRAPHICS_VULKAN_SHADERS_LINEAR_GRADIENT_PUSH_CONSTANTS_H
#define SRC_GRAPHICS_VULKAN_SHADERS_LINEAR_GRADIENT_PUSH_CONSTANTS_H

#ifdef __cplusplus
#include <glm/glm.hpp>

#include "push_constants.h"

namespace karin
{
struct LinearGradientPushConstants
{
    glm::vec2 start;
    glm::vec2 end;
    glm::vec3 shapeParams;
    ShapeType shapeType = ShapeType::Nothing;
};
}

#else // glsl

layout (push_constant) uniform LinearGradientPushConstants
{
    // it must [-1, 1] range
    vec4 coords;
    vec3 shapeParams;
    uint shapeType;
} push;

#endif

#endif //SRC_GRAPHICS_VULKAN_SHADERS_LINEAR_GRADIENT_PUSH_CONSTANTS_H
