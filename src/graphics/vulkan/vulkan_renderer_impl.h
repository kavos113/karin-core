#ifndef SRC_GRAPHICS_GRAPHICS_VULKAN_VK_RENDERER_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_VULKAN_VK_RENDERER_IMPL_H

#include "vulkan_device_resources.h"
#include "vulkan_graphics_device.h"
#include "vulkan_pipeline.h"
#include "vulkan_surface.h"
#include "shaders/push_constants.h"

#include <renderer_impl.h>
#include <karin/common/geometry/point.h>
#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/pattern.h>
#include <karin/system/window.h>

#include <vector>

namespace karin
{
class VulkanRendererImpl : public IRendererImpl
{
public:
    VulkanRendererImpl(VulkanGraphicsDevice* device, Window::NativeHandle nativeHandle);
    ~VulkanRendererImpl() override = default;

    void cleanUp() override;

    bool beginDraw() override;
    void endDraw() override;
    void resize(Size size) override;
    void setClearColor(const Color& color) override;

    void addCommand(
        const std::vector<VulkanPipeline::Vertex>& vertices,
        std::vector<uint16_t>& indices,
        const PushConstants& fragData,
        const Pattern& pattern
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
        PushConstants fragData;
        VulkanPipeline* pipeline = nullptr;
        VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
        PatternType patternType = PatternType::SolidColor;
    };

    void createCommandBuffers();
    void createSyncObjects();
    void createVertexBuffer();
    void createIndexBuffer();
    void createRenderPass();
    void createFrameBuffers();
    void createPipeline();
    void createLinearGradientPipeline();
    void createRadialGradientPipeline();

    void doResize();

    VulkanGraphicsDevice* m_device;
    std::unique_ptr<VulkanSurface> m_surface;
    std::unique_ptr<VulkanPipeline> m_pipeline;
    std::unique_ptr<VulkanPipeline> m_linearGradientPipeline;
    std::unique_ptr<VulkanPipeline> m_radialGradientPipeline;
    std::unique_ptr<VulkanDeviceResources> m_deviceResources;

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
    VulkanPipeline::Vertex* m_vertexMapPoint = nullptr;
    VulkanPipeline::Vertex* m_vertexStartPoint = nullptr;
    VkBuffer m_indexBuffer = VK_NULL_HANDLE;
    VmaAllocation m_indexAllocation = VK_NULL_HANDLE;
    uint16_t* m_indexMapPoint = nullptr;
    uint16_t* m_indexStartPoint = nullptr;
    uint16_t m_vertexOffset = 0;
    size_t m_indexCount = 0;

    static constexpr VkDeviceSize vertexBufferSize = 1024 * 128; // 2MB
    static constexpr VkDeviceSize indexBufferSize = 1024 * 512; // 2MB

    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    VkClearValue m_clearColor = {{1.0f, 1.0f, 1.0f, 1.0f}};
};
} // karin

#endif //SRC_GRAPHICS_GRAPHICS_VULKAN_VK_RENDERER_IMPL_H
