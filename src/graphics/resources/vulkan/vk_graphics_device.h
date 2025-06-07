#ifndef SRC_GRAPHICS_RESOURCES_VULKAN_VK_GRAPHICS_DEVICE_H
#define SRC_GRAPHICS_RESOURCES_VULKAN_VK_GRAPHICS_DEVICE_H

#include <map>
#include <vulkan/vulkan.h>

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

    void createLogicalDevice(VkSurfaceKHR surface);

    void cleanUp() override;

private:
    void createInstance();
    void choosePhysicalDevice();
    void createVmaAllocator();

    void getQueueFamily(VkSurfaceKHR surface);

    std::unique_ptr<VkDebugManager> m_debugManager;

    VkInstance m_instance = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VmaAllocator m_allocator = VK_NULL_HANDLE;

    // initialized when the first window surface(renderer) is created
    VkDevice m_device = VK_NULL_HANDLE;
    std::map<QueueFamily, uint32_t> m_queueFamilyIndices;

    const bool m_enableValidationLayers = true;

    static constexpr uint32_t VK_API_VERSION = VK_API_VERSION_1_0;
};

} // karin

#endif //SRC_GRAPHICS_RESOURCES_VULKAN_VK_GRAPHICS_DEVICE_H
