#ifndef SRC_GRAPHICS_VULKAN_SHADERS_PUSH_CONSTANTS_H
#define SRC_GRAPHICS_VULKAN_SHADERS_PUSH_CONSTANTS_H

#ifdef __cplusplus
#include <glm/glm.hpp>

namespace karin
{
enum class ShapeType : uint32_t
{
    Nothing = 0,
    Ellipse = 1,
    RoundedRectangle = 2,
};

struct PushConstants
{
    // color(vec4) in solid color
    // start(vec2) + end(vec2) in linear gradient
    // center(vec2) + offset(vec2) in radial gradient
    glm::vec4 color;

    glm::vec3 shapeParams;
    ShapeType shapeType = ShapeType::Nothing;

    // aspect(float) in linear gradient (width / height)
    // radiusX(float) + radiusY(float) in radial gradient
    glm::vec2 global;
};
}

#else // glsl

layout (push_constant) uniform PushConstants
{
    vec4 color;
    vec3 shapeParams;
    uint shapeType;
    vec2 global;
} push;

#endif


#endif //SRC_GRAPHICS_VULKAN_SHADERS_PUSH_CONSTANTS_H
