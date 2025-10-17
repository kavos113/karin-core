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
#include <cstdint>
#include <memory>

namespace karin
{
class VulkanRendererImpl : public IRendererImpl
{
public:
    VulkanRendererImpl(
        VulkanGraphicsDevice* device,
        Window::NativeHandle nativeHandle,
        std::unique_ptr<FontLoader> fontLoader
    );
    ~VulkanRendererImpl() override = default;

    void cleanUp() override;

    bool beginDraw() override;
    void endDraw() override;
    void resize(Size size) override;

    void setClearColor(const Color& color) override
    {
        m_clearColor = {
            .color = {
                .float32 = {color.r, color.g, color.b, color.a}
            }
        };
    }

    void addCommand(
        const std::vector<VulkanPipeline::Vertex>& vertices,
        std::vector<uint16_t>& indices,
        const PushConstants& fragData,
        const Pattern& pattern, bool isGeometry
    );

    Image createImage(const std::vector<std::byte>& data, uint32_t width, uint32_t height) override
    {
        return m_deviceResources->createImage(data, width, height);
    }

    // pixel coordinates -> normalized coordinates [-1, 1]
    Rectangle normalize(Rectangle rect) const
    {
        VkExtent2D extent = m_surface->extent();

        return {
            {
                (rect.pos.x / static_cast<float>(extent.width)) * 2.0f - 1.0f,
                (rect.pos.y / static_cast<float>(extent.height)) * 2.0f - 1.0f
            },
            {
                (rect.size.width / static_cast<float>(extent.width)) * 2.0f,
                (rect.size.height / static_cast<float>(extent.height)) * 2.0f
            }
        };
    }

    Point normalize(Point point) const
    {
        VkExtent2D extent = m_surface->extent();

        return {
            (point.x / static_cast<float>(extent.width)) * 2.0f - 1.0f,
            (point.y / static_cast<float>(extent.height)) * 2.0f - 1.0f
        };
    }

    glm::vec2 normalize(glm::vec2 v) const
    {
        VkExtent2D extent = m_surface->extent();

        return {
            (v.x / static_cast<float>(extent.width)) * 2.0f - 1.0f,
            (v.y / static_cast<float>(extent.height)) * 2.0f - 1.0f
        };
    }

    glm::vec2 unNormalize(glm::vec2 v) const
    {
        VkExtent2D extent = m_surface->extent();

        return {
            (v.x + 1.0f) / 2.0f * static_cast<float>(extent.width),
            (v.y + 1.0f) / 2.0f * static_cast<float>(extent.height)
        };
    }

    // only change scale
    glm::vec2 normalizeVec(glm::vec2 vec) const
    {
        VkExtent2D extent = m_surface->extent();

        return {
            (vec.x / static_cast<float>(extent.width)) * 2.0f,
            (vec.y / static_cast<float>(extent.height)) * 2.0f
        };
    }

    Point normalizeVec(Point vec) const
    {
        VkExtent2D extent = m_surface->extent();

        return {
            (vec.x / static_cast<float>(extent.width)) * 2.0f,
            (vec.y / static_cast<float>(extent.height)) * 2.0f
        };
    }

    VulkanDeviceResources* deviceResources() const
    {
        return m_deviceResources.get();
    }

private:
    struct DrawCommand
    {
        uint32_t indexCount{};
        uint32_t indexOffset{};
        PushConstants fragData;
        VulkanPipeline* pipeline = nullptr;
        std::vector<VkDescriptorSet> descriptorSets;
    };

    struct MatrixBufferObject
    {
        glm::mat4 proj;
    };

    void createCommandBuffers();
    void createSyncObjects();
    void createVertexBuffer();
    void createIndexBuffer();
    void createMatrixBuffer();
    void createRenderPass();
    void createFrameBuffers();
    void createPipeline();

    void doResize();

    VulkanGraphicsDevice* m_device;
    std::unique_ptr<VulkanSurface> m_surface;
    std::unique_ptr<VulkanPipeline> m_geometryPipeline;
    std::unique_ptr<VulkanPipeline> m_textPipeline;
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

    MatrixBufferObject m_projMatrixData = {};
    VkDescriptorSetLayout m_projMatrixDescriptorSetLayout = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> m_projMatrixDescriptorSets;
    std::vector<VkBuffer> m_projMatrixBuffers;
    std::vector<VmaAllocation> m_projMatrixBufferAllocations;
    std::vector<VmaAllocationInfo> m_projMatrixBufferMemoryInfos;

    static constexpr VkDeviceSize vertexBufferSize = 1024 * 128; // 2MB
    static constexpr VkDeviceSize indexBufferSize = 1024 * 512; // 2MB

    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    VkClearValue m_clearColor = {{1.0f, 1.0f, 1.0f, 1.0f}};
};
} // karin

#endif //SRC_GRAPHICS_GRAPHICS_VULKAN_VK_RENDERER_IMPL_H