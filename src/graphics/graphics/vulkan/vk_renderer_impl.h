#ifndef SRC_GRAPHICS_GRAPHICS_VULKAN_VK_RENDERER_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_VULKAN_VK_RENDERER_IMPL_H

#include <graphics/renderer_impl.h>

#include <resources/vulkan/vk_graphics_device.h>
#include <resources/vulkan/vk_surface_impl.h>

#include <karin/common/geometry/rectangle.h>

#include "vk_pipeline_manager.h"

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

    void addBuffer(const std::vector<VkPipelineManager::Vertex> &vertices, std::vector<uint16_t> &indices);

    // pixel coordinates -> normalized coordinates [-1, 1]
    Rectangle normalize(Rectangle rect) const;
private:
    void createCommandBuffers();
    void createSyncObjects();
    void createVertexBuffer();
    void createIndexBuffer();
    void createRenderPass();
    void createFrameBuffers();

    VkGraphicsDevice* m_device;
    VkSurfaceImpl* m_surface;
    std::unique_ptr<VkPipelineManager> m_pipelineManager;

    uint8_t m_currentFrame = 0;
    uint32_t m_imageIndex = 0;

    std::vector<VkFramebuffer> m_swapChainFramebuffers;
    std::vector<VkCommandBuffer> m_commandBuffers;
    std::vector<VkSemaphore> m_finishQueueSemaphores;
    std::vector<VkSemaphore> m_swapChainSemaphores;
    std::vector<VkFence> m_swapChainFences;

    VkRenderPass m_renderPass = VK_NULL_HANDLE;

    VkExtent2D m_extent;

    VkBuffer m_vertexBuffer;
    VmaAllocation m_vertexAllocation;
    void* m_vertexMapPoint;
    void* m_vertexStartPoint;
    VkBuffer m_indexBuffer;
    VmaAllocation m_indexAllocation;
    void* m_indexMapPoint;
    void* m_indexStartPoint;
    uint16_t m_vertexOffset;
    size_t m_indexCount;

    static constexpr VkDeviceSize vertexBufferSize = 1024 * 128; // 2MB
    static constexpr VkDeviceSize indexBufferSize = 1024 * 512; // 2MB

    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    VkClearValue m_clearColor = { { 1.0f, 1.0f, 1.0f, 1.0f } };
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_VULKAN_VK_RENDERER_IMPL_H
