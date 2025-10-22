#ifndef SRC_GRAPHICS_VULKAN_SHADERS_PUSH_CONSTANTS_H
#define SRC_GRAPHICS_VULKAN_SHADERS_PUSH_CONSTANTS_H

#ifdef __cplusplus
#include <glm/glm.hpp>
#include <karin/graphics/pattern.h>

namespace karin
{
enum class ShapeType : uint32_t
{
    Nothing = 0,
    Ellipse = 1,
    RoundedRectangle = 2,
};

struct FragPushConstants
{
    // color(vec4) in solid color
    // start(vec2) + end(vec2) in linear gradient
    // center(vec2) + offset(vec2) in radial gradient
    // offset(vec2) + scale(vec2) in image
    glm::vec4 color;

    // only used in rounded rectangle. vec2: normalized radius (radiusX / (width / 2), radiusY / (height / 2))
    glm::vec2 shapeParams;
    uint32_t shapeType = static_cast<uint32_t>(ShapeType::Nothing);
    uint32_t patternType = static_cast<uint32_t>(PatternType::SolidColor);

    // radiusX(float) + radiusY(float) in radial gradient
    // imageSize(vec2), uvMode(float) in image (0 = uv(image), 1 = window coordinates(image pattern))
    glm::vec4 patternParams;
};

struct VertexPushConstants
{
    glm::mat4 model;
};
}

#else // glsl

#ifdef FRAGMENT_SHADER

layout (push_constant) uniform PushConstants
{
    vec4 color;
    vec2 shapeParams;
    uint shapeType;
    uint patternType;
    vec4 patternParams;
} push;

#elif defined(VERTEX_SHADER)

layout (push_constant) uniform TransformMatrix
{
    layout(offset = 48) mat4 model;
} push;

#endif

#endif


#endif //SRC_GRAPHICS_VULKAN_SHADERS_PUSH_CONSTANTS_H