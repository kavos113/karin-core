#ifndef SRC_GRAPHICS_RESOURCES_VULKAN_VK_DEVICE_UTILS_H
#define SRC_GRAPHICS_RESOURCES_VULKAN_VK_DEVICE_UTILS_H
#include <vulkan/vulkan.h>
#include <vector>

namespace karin
{

class VkDeviceUtils
{
public:
    static bool checkValidationLayerSupport();
    static int rateDeviceScore(VkPhysicalDevice device);

    static const std::vector<const char*> VALIDATION_LAYERS;
    static const std::vector<const char*> DEVICE_EXTENSIONS;
private:
    static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
};

} // karin

#endif //SRC_GRAPHICS_RESOURCES_VULKAN_VK_DEVICE_UTILS_H
