#ifndef SRC_GRAPHICS_RESOURCES_VULKAN_VK_DEBUG_MANAGER_H
#define SRC_GRAPHICS_RESOURCES_VULKAN_VK_DEBUG_MANAGER_H

#include <vulkan/vulkan.h>

namespace karin
{

class VulkanDebugManager
{
public:
    explicit VulkanDebugManager(VkInstance instance);
    ~VulkanDebugManager();

    void cleanup(VkInstance instance);
    void addDebugSupportToInstance(VkInstanceCreateInfo &createInfo, VkDebugUtilsMessengerCreateInfoEXT &debugCreateInfo) const;
    void addDebugSupportToDevice(VkDeviceCreateInfo &createInfo) const;
private:
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
};

} // karin

#endif //SRC_GRAPHICS_RESOURCES_VULKAN_VK_DEBUG_MANAGER_H
