#ifndef SRC_GRAPHICS_RESOURCES_VULKAN_VK_DEBUG_MANAGER_H
#define SRC_GRAPHICS_RESOURCES_VULKAN_VK_DEBUG_MANAGER_H

#include <vector>
#include <vulkan/vulkan.h>

namespace karin
{

class VkDebugManager
{
public:
    explicit VkDebugManager(VkInstance instance);
    ~VkDebugManager();

    void cleanup(VkInstance instance);
    void addDebugSupportToInstance(VkInstanceCreateInfo &createInfo, VkDebugUtilsMessengerCreateInfoEXT &debugCreateInfo) const;
    void addDebugSupportToDevice(VkDeviceCreateInfo &createInfo) const;
private:
    bool checkValidationLayerSupport() const;

    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };
};

} // karin

#endif //SRC_GRAPHICS_RESOURCES_VULKAN_VK_DEBUG_MANAGER_H
