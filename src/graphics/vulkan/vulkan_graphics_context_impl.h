#ifndef SRC_GRAPHICS_GRAPHICS_VULKAN_VK_GRAPHICS_CONTEXT_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_VULKAN_VK_GRAPHICS_CONTEXT_IMPL_H

#include "vulkan_renderer_impl.h"

#include <graphics_context_impl.h>
#include <karin/graphics/stroke_style.h>
#include <karin/graphics/path.h>
#include <karin/common/color/pattern.h>
#include <karin/common/geometry/point.h>
#include <karin/common/geometry/rectangle.h>

namespace karin {

class VulkanGraphicsContextImpl : public IGraphicsContextImpl
{
public:
    VulkanGraphicsContextImpl(VulkanRendererImpl *renderer);
    ~VulkanGraphicsContextImpl() override = default;

    void fillRect(Rectangle rect, Pattern *pattern) override;
    void fillEllipse(Point center, float radiusX, float radiusY, Pattern *pattern) override;
    void fillRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern *pattern) override;
    void fillPath(const Path& path, Pattern* pattern) override;
    void drawLine(Point start, Point end, Pattern *pattern, const StrokeStyle& strokeStyle) override;
    void drawRect(Rectangle rect, Pattern *pattern, const StrokeStyle& strokeStyle) override;
    void drawEllipse(Point center, float radiusX, float radiusY, Pattern *pattern, const StrokeStyle& strokeStyle) override;
    void drawRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern *pattern, const StrokeStyle& strokeStyle) override;
    void drawPath(const Path& path, Pattern* pattern, const StrokeStyle& strokeStyle) override;

private:
    // return dash_offset for next line
    float addLine(
        Point start,
        Point end,
        const StrokeStyle& strokeStyle,
        std::vector<VulkanPipeline::Vertex>& vertices,
        std::vector<uint16_t>& indices
    ) const;

    void addCapStyle(
        StrokeStyle::CapStyle capStyle,
        std::vector<VulkanPipeline::Vertex>& vertices,
        std::vector<uint16_t>& indices,
        const glm::vec2& centerVec,
        const Point& direction,
        float width, // pixel
        glm::vec2 dirUnitVec,
        glm::vec2 normalUnitVec
    ) const;

    float addArc(
        Point center,
        float radiusX,
        float radiusY,
        float startAngle,
        float endAngle,
        const StrokeStyle& strokeStyle,
        std::vector<VulkanPipeline::Vertex>& vertices,
        std::vector<uint16_t>& indices
    ) const;

    // should start < end
    std::vector<Point> splitArc(
        Point center,
        float radiusX,
        float radiusY,
        float startAngle,
        float endAngle
    ) const;

    VulkanRendererImpl* m_renderer;

    static constexpr int CAP_ROUND_SEGMENTS = 8;
    static constexpr int ELLIPSE_SEGMENTS = 32;
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_VULKAN_VK_GRAPHICS_CONTEXT_IMPL_H
