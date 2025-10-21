#ifndef SRC_GRAPHICS_GRAPHICS_VULKAN_VK_GRAPHICS_CONTEXT_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_VULKAN_VK_GRAPHICS_CONTEXT_IMPL_H

#include "vulkan_renderer_impl.h"
#include "shaders/push_constants.h"

#include <graphics_context_impl.h>
#include <path_impl.h>

#include <karin/common/geometry/point.h>
#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/stroke_style.h>

namespace karin
{
class VulkanGraphicsContextImpl : public IGraphicsContextImpl
{
public:
    explicit VulkanGraphicsContextImpl(VulkanRendererImpl* renderer);
    ~VulkanGraphicsContextImpl() override = default;

    void fillRect(Rectangle rect, Pattern& pattern, const Transform2D& transform) override;
    void fillEllipse(
        Point center, float radiusX, float radiusY, Pattern& pattern, const Transform2D& transform
    ) override;
    void fillRoundedRect(
        Rectangle rect, float radiusX, float radiusY, Pattern& pattern, const Transform2D& transform
    ) override;
    void fillPath(const PathImpl& path, Pattern& pattern, const Transform2D& transform) override;
    void drawLine(
        Point start, Point end, Pattern& pattern, const StrokeStyle& strokeStyle, const Transform2D& transform
    ) override;
    void drawRect(
        Rectangle rect, Pattern& pattern, const StrokeStyle& strokeStyle, const Transform2D& transform
    ) override;
    void drawEllipse(
        Point center, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle, const Transform2D&
        transform
    ) override;
    void drawRoundedRect(
        Rectangle rect, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle,
        const Transform2D&
        transform
    ) override;
    void drawPath(
        const PathImpl& path, Pattern& pattern, const StrokeStyle& strokeStyle, const Transform2D& transform
    ) override;

    void drawImage(
        Image image, Rectangle destRect, Rectangle srcRect, float opacity, const Transform2D& transform
    ) override;
    void drawText(const TextLayout& text, Point start, Pattern& pattern, const Transform2D& transform) override;

private:
    FragPushConstants createPushConstantData(const Pattern& pattern) const;

    VulkanRendererImpl* m_renderer;

    static constexpr int CAP_ROUND_SEGMENTS = 8;
    static constexpr int ELLIPSE_SEGMENTS = 32;
};
} // karin

#endif //SRC_GRAPHICS_GRAPHICS_VULKAN_VK_GRAPHICS_CONTEXT_IMPL_H
