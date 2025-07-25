#ifndef SRC_GRAPHICS_GRAPHICS_VULKAN_VK_RENDERER_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_VULKAN_VK_RENDERER_IMPL_H

#include <graphics/renderer_impl.h>

#include <resources/vulkan/vk_graphics_device.h>
#include <resources/vulkan/vk_surface_manager.h>
#include <x11/window.h>

#include <karin/common/geometry/rectangle.h>

#include "vk_pipeline_manager.h"

namespace karin {

class VkRendererImpl : public IRendererImpl
{
public:
    VkRendererImpl(VkGraphicsDevice* device, XlibWindow window, Display* display);
    ~VkRendererImpl() override = default;

    void cleanUp() override;

    bool beginDraw() override;
    void endDraw() override;
    void resize(Size size) override;
    void setClearColor(const Color &color) override;

    void addCommand(
        const std::vector<VkPipelineManager::Vertex> &vertices,
        std::vector<uint16_t> &indices,
        const VkPipelineManager::FragPushConstantData &fragData
    );

    // pixel coordinates -> normalized coordinates [-1, 1]
    Rectangle normalize(Rectangle rect) const;
    Point normalize(Point point) const;
    glm::vec2 normalize(glm::vec2 v) const;
    glm::vec2 unNormalize(glm::vec2 v) const;
    // only change scale
    glm::vec2 normalizeVec(glm::vec2 vec) const;
private:
    struct DrawCommand
    {
        uint32_t indexCount{};
        uint32_t indexOffset{};
        VkPipelineManager::FragPushConstantData fragData;
    };

    void createCommandBuffers();
    void createSyncObjects();
    void createVertexBuffer();
    void createIndexBuffer();
    void createRenderPass();
    void createFrameBuffers();

    void doResize();

    VkGraphicsDevice* m_device;
    std::unique_ptr<VkSurfaceManager> m_surface;
    std::unique_ptr<VkPipelineManager> m_pipelineManager;

    std::vector<DrawCommand> m_drawCommands;

    uint8_t m_currentFrame = 0;
    uint32_t m_imageIndex = 0;

    std::vector<VkFramebuffer> m_swapChainFramebuffers;
    std::vector<VkCommandBuffer> m_commandBuffers;
    std::vector<VkSemaphore> m_finishQueueSemaphores;
    std::vector<VkSemaphore> m_swapChainSemaphores;
    std::vector<VkFence> m_swapChainFences;

    VkRenderPass m_renderPass = VK_NULL_HANDLE;

    VkExtent2D m_extent = {};

    VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
    VmaAllocation m_vertexAllocation = VK_NULL_HANDLE;
    void* m_vertexMapPoint = nullptr;
    void* m_vertexStartPoint = nullptr;
    VkBuffer m_indexBuffer = VK_NULL_HANDLE;
    VmaAllocation m_indexAllocation = VK_NULL_HANDLE;
    void* m_indexMapPoint = nullptr;
    void* m_indexStartPoint = nullptr;
    uint16_t m_vertexOffset = 0;
    size_t m_indexCount = 0;

    static constexpr VkDeviceSize vertexBufferSize = 1024 * 128; // 2MB
    static constexpr VkDeviceSize indexBufferSize = 1024 * 512; // 2MB

    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    VkClearValue m_clearColor = { { 1.0f, 1.0f, 1.0f, 1.0f } };
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_VULKAN_VK_RENDERER_IMPL_H
