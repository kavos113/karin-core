#ifndef SRC_GRAPHICS_VULKAN_VULKAN_TESSELATOR_H
#define SRC_GRAPHICS_VULKAN_VULKAN_TESSELATOR_H

#include "vulkan_renderer_impl.h"
#include "vulkan_pipeline.h"

#include <karin/common/geometry/point.h>
#include <karin/graphics/stroke_style.h>

#include <glm/glm.hpp>

#include <vector>

namespace karin
{
class VulkanTessellator
{
public:
    explicit VulkanTessellator(VulkanRendererImpl* renderer)
        : m_renderer(renderer)
    {
    }

    // return dash_offset for next line
    // point: not normalized (in pixels)
    float addLine(
        Point start,
        Point end,
        const StrokeStyle& strokeStyle,
        std::vector<VulkanPipeline::Vertex>& vertices,
        std::vector<uint16_t>& indices
    ) const;

    float addArc(
        Point center,
        float radiusX,
        float radiusY,
        float startAngle,
        float endAngle,
        bool isClockwise,
        const StrokeStyle& strokeStyle,
        std::vector<VulkanPipeline::Vertex>& vertices,
        std::vector<uint16_t>& indices
    ) const;

    // clockwise: start < end
    static std::vector<Point> splitArc(
        Point center,
        float radiusX,
        float radiusY,
        float startAngle,
        float endAngle,
        bool isClockwise
    );

    static std::vector<uint16_t> triangulate(const std::vector<Point>& polygon);

private:
    void addCapStyle(
        StrokeStyle::CapStyle capStyle,
        std::vector<VulkanPipeline::Vertex>& vertices,
        std::vector<uint16_t>& indices,
        const glm::vec2& centerVec,
        const glm::vec2& dirUnitVec,
        const glm::vec2& normalVec,
        float width
    ) const;

    VulkanRendererImpl* m_renderer;

    static constexpr int CAP_ROUND_SEGMENTS = 8;
    static constexpr int ELLIPSE_SEGMENTS = 32;
};
} // karin

#endif //SRC_GRAPHICS_VULKAN_VULKAN_TESSELATOR_H
