#ifndef SRC_GRAPHICS_GRAPHICS_VULKAN_VK_GRAPHICS_CONTEXT_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_VULKAN_VK_GRAPHICS_CONTEXT_IMPL_H

#include <graphics/graphics_context_impl.h>

#include <karin/graphics/graphics/stroke_style.h>

#include "vk_renderer_impl.h"

namespace karin {

class VkGraphicsContextImpl : public IGraphicsContextImpl
{
public:
    VkGraphicsContextImpl(VkRendererImpl *renderer);
    ~VkGraphicsContextImpl() override = default;

    void fillRect(Rectangle rect, Pattern *pattern) override;
    void fillEllipse(Point center, float radiusX, float radiusY, Pattern *pattern) override;
    void fillRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern *pattern) override;
    void drawLine(Point start, Point end, Pattern *pattern, const StrokeStyle& strokeStyle) override;
    void drawRect(Rectangle rect, Pattern *pattern, const StrokeStyle& strokeStyle) override;
    void drawEllipse(Point center, float radiusX, float radiusY, Pattern *pattern, const StrokeStyle& strokeStyle) override;
    void drawRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern *pattern, const StrokeStyle& strokeStyle) override;

private:
    // return dash_offset for next line
    float addLine(
        Point start,
        Point end,
        const StrokeStyle& strokeStyle,
        std::vector<VkPipelineManager::Vertex>& vertices,
        std::vector<uint16_t>& indices
    ) const;

    void addCapStyle(
        StrokeStyle::CapStyle capStyle,
        std::vector<VkPipelineManager::Vertex>& vertices,
        std::vector<uint16_t>& indices,
        const glm::vec2& centerVec,
        const Point& direction,
        float width, // pixel
        glm::vec2 dirUnitVec,
        glm::vec2 normalUnitVec
    ) const;

    VkRendererImpl* m_renderer;

    static constexpr int ROUND_SEGMENTS = 16;
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_VULKAN_VK_GRAPHICS_CONTEXT_IMPL_H
