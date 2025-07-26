#ifndef SRC_GRAPHICS_RESOURCES_VULKAN_VK_DEVICE_UTILS_H
#define SRC_GRAPHICS_RESOURCES_VULKAN_VK_DEVICE_UTILS_H
#include <vulkan/vulkan.h>

#include <vector>

namespace karin
{

class VkUtils
{
public:
    static bool checkValidationLayerSupport();
    static int rateDeviceScore(VkPhysicalDevice device);

    static VkSurfaceFormatKHR getBestSwapSurfaceFormat(VkPhysicalDevice device, VkSurfaceKHR surface);
    static VkPresentModeKHR getBestSwapPresentMode(VkPhysicalDevice device, VkSurfaceKHR surface);
    static VkSurfaceCapabilitiesKHR getSwapCapabilities(VkPhysicalDevice device, VkSurfaceKHR surface);
    static VkExtent2D getSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, int width, int height);

    static VkShaderModule loadShader(VkDevice device, const unsigned char* code, unsigned int codeSize);

    static const std::vector<const char*> VALIDATION_LAYERS;
    static const std::vector<const char*> DEVICE_EXTENSIONS;
private:
    static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
};

} // karin

#endif //SRC_GRAPHICS_RESOURCES_VULKAN_VK_DEVICE_UTILS_H
