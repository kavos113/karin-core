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

struct SolidPushConstants
{
    glm::vec4 color;
    glm::vec3 shapeParams;
    ShapeType shapeType = ShapeType::Nothing;
};
}

#else // glsl

layout (push_constant) uniform PushConstants
{
    vec4 color;
    vec3 shapeParams;
    uint shapeType;
} pushConstants;

#endif


#endif //SRC_GRAPHICS_VULKAN_SHADERS_PUSH_CONSTANTS_H
