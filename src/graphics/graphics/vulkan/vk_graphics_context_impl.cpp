#include "vk_graphics_context_impl.h"

#include <iostream>
#include <resources/vulkan/vk_graphics_device.h>

namespace karin
{
VkGraphicsContextImpl::VkGraphicsContextImpl(VkRendererImpl *renderer)
    : m_renderer(renderer)
{
}

void VkGraphicsContextImpl::fillRect(Rectangle rect, Pattern *pattern, float strokeWidth)
{
    Rectangle normalizedRect = m_renderer->normalize(rect);

    std::cout << "Normalized Rectangle: "
              << "Position(" << normalizedRect.pos.x << ", " << normalizedRect.pos.y << "), "
              << "Size(" << normalizedRect.size.width << ", " << normalizedRect.size.height << ")"
              << std::endl;

    std::vector<VkGraphicsDevice::Vertex> vertices = {
        {
            {normalizedRect.pos.x, normalizedRect.pos.y},
            {1.0f, 0.0f, 0.0f}
        },
        {
            {normalizedRect.pos.x + normalizedRect.size.width, normalizedRect.pos.y},
            {0.0f, 1.0f, 0.0f}
        },
        {
            {normalizedRect.pos.x + normalizedRect.size.width, normalizedRect.pos.y + normalizedRect.size.height},
            {0.0f, 0.0f, 1.0f}
        },
        {
            {normalizedRect.pos.x, normalizedRect.pos.y + normalizedRect.size.height},
            {1.0f, 1.0f, 1.0f}
        }
    };

    std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    // TODO
    m_renderer->addBuffer(vertices, indices);
}
} // karin