#ifndef VK_GRAPHICS_CONTEXT_IMPL_H
#define VK_GRAPHICS_CONTEXT_IMPL_H

#include <graphics/graphics_context_impl.h>

#include "vk_renderer_impl.h"

namespace karin {

class VkGraphicsContextImpl : public IGraphicsContextImpl
{
public:
    VkGraphicsContextImpl(VkRendererImpl *renderer);
    ~VkGraphicsContextImpl() override = default;

    void fillRect(Rectangle rect, Pattern *pattern) override;

private:
    VkRendererImpl* m_renderer;
};

} // karin

#endif //VK_GRAPHICS_CONTEXT_IMPL_H
