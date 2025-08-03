#ifndef SRC_GRAPHICS_GRAPHICS_VULKAN_VK_GRAPHICS_CONTEXT_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_VULKAN_VK_GRAPHICS_CONTEXT_IMPL_H

#include "vulkan_renderer_impl.h"
#include "vulkan_tessellator.h"

#include <graphics_context_impl.h>
#include <path_impl.h>

#include <karin/graphics/stroke_style.h>
#include <karin/common/color/pattern.h>
#include <karin/common/geometry/point.h>
#include <karin/common/geometry/rectangle.h>

#include <glm/glm.hpp>

#include <memory>

namespace karin
{
class VulkanGraphicsContextImpl : public IGraphicsContextImpl
{
public:
    explicit VulkanGraphicsContextImpl(VulkanRendererImpl* renderer);
    ~VulkanGraphicsContextImpl() override = default;

    void fillRect(Rectangle rect, Pattern& pattern) override;
    void fillEllipse(Point center, float radiusX, float radiusY, Pattern& pattern) override;
    void fillRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern& pattern) override;
    void fillPath(const PathImpl& path, Pattern& pattern) override;
    void drawLine(Point start, Point end, Pattern& pattern, const StrokeStyle& strokeStyle) override;
    void drawRect(Rectangle rect, Pattern& pattern, const StrokeStyle& strokeStyle) override;
    void drawEllipse(
        Point center, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle
    ) override;
    void drawRoundedRect(
        Rectangle rect, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle
    ) override;
    void drawPath(const PathImpl& path, Pattern& pattern, const StrokeStyle& strokeStyle) override;

private:
    VulkanRendererImpl* m_renderer;

    std::unique_ptr<VulkanTessellator> m_tessellator;

    static constexpr int CAP_ROUND_SEGMENTS = 8;
    static constexpr int ELLIPSE_SEGMENTS = 32;
};
} // karin

#endif //SRC_GRAPHICS_GRAPHICS_VULKAN_VK_GRAPHICS_CONTEXT_IMPL_H
