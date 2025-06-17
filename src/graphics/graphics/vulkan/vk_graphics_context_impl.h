#ifndef VK_GRAPHICS_CONTEXT_IMPL_H
#define VK_GRAPHICS_CONTEXT_IMPL_H

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
    VkRendererImpl* m_renderer;
};

} // karin

#endif //VK_GRAPHICS_CONTEXT_IMPL_H
