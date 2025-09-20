#ifndef SRC_GRAPHICS_RESOURCES_VULKAN_VK_GRAPHICS_DEVICE_H
#define SRC_GRAPHICS_RESOURCES_VULKAN_VK_GRAPHICS_DEVICE_H

#include "vma.h"
#include "vulkan_debug_manager.h"

#include <karin/graphics/graphics_device.h>

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <map>

namespace karin
{
// TODO? options for enabling/disabling validation layers, debug utils, etc.
class VulkanGraphicsDevice : public GraphicsDevice
{
public:
    enum class QueueFamily : uint8_t
    {
        Graphics,
        Present
    };

    VulkanGraphicsDevice();
    ~VulkanGraphicsDevice() override;

    void initDevices(VkSurfaceKHR surface);

    void cleanUp() override;

    VkCommandBuffer beginSingleTimeCommands() const;
    void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;

    VkInstance instance() const
    {
        return m_instance;
    }

    VkPhysicalDevice physicalDevice() const
    {
        return m_physicalDevice;
    }

    VkDevice device() const
    {
        return m_device;
    }

    VmaAllocator allocator() const
    {
        return m_allocator;
    }

    uint32_t queueFamilyIndex(QueueFamily family) const
    {
        return m_queueFamilyIndices.at(family);
    }

    VkCommandPool commandPool() const
    {
        return m_commandPool;
    }

    VkQueue graphicsQueue() const
    {
        return m_graphicsQueue;
    }

    VkQueue presentQueue() const
    {
        return m_presentQueue;
    }

    VkDescriptorPool descriptorPool() const
    {
        return m_descriptorPool;
    }

private:
    void createInstance();
    void choosePhysicalDevice();
    void createVmaAllocator();

    void createLogicalDevice();
    void getQueueFamily(VkSurfaceKHR surface);
    void createCommandPool();
    void createDescriptorPool();

    std::unique_ptr<VulkanDebugManager> m_debugManager;

    VkInstance m_instance = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VmaAllocator m_allocator = VK_NULL_HANDLE;

    // initialized when the first window surface(renderer) is created
    VkDevice m_device = VK_NULL_HANDLE;
    std::map<QueueFamily, uint32_t> m_queueFamilyIndices;
    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkQueue m_presentQueue = VK_NULL_HANDLE;

    VkCommandPool m_commandPool = VK_NULL_HANDLE;
    VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;

    const bool m_enableValidationLayers = true;

    static constexpr uint32_t VK_API_VERSION = VK_API_VERSION_1_0;
};
} // karin

#endif //SRC_GRAPHICS_RESOURCES_VULKAN_VK_GRAPHICS_DEVICE_H