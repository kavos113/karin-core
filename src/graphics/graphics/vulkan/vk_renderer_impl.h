#ifndef VK_RENDERER_IMPL_H
#define VK_RENDERER_IMPL_H

#include <graphics/renderer_impl.h>

#include <resources/vulkan/vk_graphics_device.h>
#include <resources/vulkan/vk_surface_impl.h>

#include <karin/common/geometry/rectangle.h>

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

    void addBuffer(std::vector<VkGraphicsDevice::Vertex> vertices, std::vector<uint16_t> indices);

    // pixel coordinates -> normalized coordinates [-1, 1]
    Rectangle normalize(Rectangle rect);

    VkSemaphore finishQueueSemaphore() const;

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
    void* m_vertexMapPoint;
    VkBuffer m_indexBuffer;
    VmaAllocation m_indexAllocation;
    void* m_indexMapPoint;
    uint16_t m_vertexCount;

    static constexpr VkDeviceSize vertexBufferSize = 1024 * 128; // 2MB
    static constexpr VkDeviceSize indexBufferSize = 1024 * 512; // 2MB

    VkClearValue m_clearColor = { { 1.0f, 1.0f, 1.0f, 1.0f } };
};

} // karin

#endif //VK_RENDERER_IMPL_H
