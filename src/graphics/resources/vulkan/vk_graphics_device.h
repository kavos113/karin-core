#ifndef SRC_GRAPHICS_RESOURCES_VULKAN_VK_GRAPHICS_DEVICE_H
#define SRC_GRAPHICS_RESOURCES_VULKAN_VK_GRAPHICS_DEVICE_H

#include <array>
#include <map>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include <karin/graphics/resources/graphics_device.h>

#include <vulkan/vma.h>

#include "vk_debug_manager.h"

namespace karin
{

// TODO? options for enabling/disabling validation layers, debug utils, etc.
class VkGraphicsDevice : public GraphicsDevice
{
public:
    enum class QueueFamily : uint8_t
    {
        Graphics,
        Present
    };

    VkGraphicsDevice();
    ~VkGraphicsDevice() override;

    void initDevices(VkSurfaceKHR surface);

    void cleanUp() override;

    VkInstance instance() const;
    VkPhysicalDevice physicalDevice() const;
    VkDevice device() const;
    VmaAllocator allocator() const;
    uint32_t queueFamilyIndex(QueueFamily family) const;
    VkRenderPass renderPass() const;
    VkCommandPool commandPool() const;
    VkPipeline graphicsPipeline() const;
    VkQueue graphicsQueue() const;
    VkQueue presentQueue() const;

    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription = {
                .binding = 0,
                .stride = sizeof(Vertex),
                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
            };
            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
        {
            std::array attributeDescriptions = {
                VkVertexInputAttributeDescription{
                    .location = 0,
                    .binding = 0,
                    .format = VK_FORMAT_R32G32_SFLOAT,
                    .offset = offsetof(Vertex, pos)
                },
                VkVertexInputAttributeDescription{
                    .location = 1,
                    .binding = 0,
                    .format = VK_FORMAT_R32G32B32_SFLOAT,
                    .offset = offsetof(Vertex, color)
                }
            };
            return attributeDescriptions;
        }
    };

private:

    void createInstance();
    void choosePhysicalDevice();
    void createVmaAllocator();

    void createLogicalDevice(VkSurfaceKHR surface);
    void getQueueFamily(VkSurfaceKHR surface);
    void createCommandPool();
    void createRenderPass();
    void createPipeline();

    std::unique_ptr<VkDebugManager> m_debugManager;

    VkInstance m_instance = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VmaAllocator m_allocator = VK_NULL_HANDLE;

    // initialized when the first window surface(renderer) is created
    VkDevice m_device = VK_NULL_HANDLE;
    std::map<QueueFamily, uint32_t> m_queueFamilyIndices;
    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkQueue m_presentQueue = VK_NULL_HANDLE;

    VkRenderPass m_renderPass = VK_NULL_HANDLE;
    VkCommandPool m_commandPool = VK_NULL_HANDLE;

    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;

    const bool m_enableValidationLayers = true;

    static constexpr uint32_t VK_API_VERSION = VK_API_VERSION_1_0;
};

} // karin

#endif //SRC_GRAPHICS_RESOURCES_VULKAN_VK_GRAPHICS_DEVICE_H
