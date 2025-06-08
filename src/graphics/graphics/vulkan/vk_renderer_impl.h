#ifndef VK_RENDERER_IMPL_H
#define VK_RENDERER_IMPL_H

#include <graphics/renderer_impl.h>

#include <resources/vulkan/vk_graphics_device.h>
#include <resources/vulkan/vk_surface_impl.h>

namespace karin {

class VkRendererImpl : public IRendererImpl
{
public:
    VkRendererImpl(VkGraphicsDevice* device, VkSurfaceImpl* surface);
    ~VkRendererImpl() override = default;

    void cleanUp() override;

    void beginDraw() override;
    void endDraw() override;
    void resize(Size size) override;
    void reset() override;

private:
    void createCommandBuffers();
    void createSemaphores();
    void createVertexBuffer();
    void createIndexBuffer();

    VkGraphicsDevice* m_device;
    VkSurfaceImpl* m_surface;

    std::vector<VkCommandBuffer> m_commandBuffers;
    std::vector<VkSemaphore> m_finishQueueSemaphores;

    VkBuffer m_vertexBuffer;
    VmaAllocation m_vertexAllocation;
    VkBuffer m_indexBuffer;
    VmaAllocation m_indexAllocation;

    static constexpr VkDeviceSize vertexBufferSize = 1024 * 1024; // 1 MB
    static constexpr VkDeviceSize indexBufferSize = 1024 * 1024; // 1 MB

    VkClearValue m_clearColor = { { 1.0f, 1.0f, 1.0f, 1.0f } };
};

} // karin

#endif //VK_RENDERER_IMPL_H
