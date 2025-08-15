#ifndef SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H
#define SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H

#include "vulkan_graphics_device.h"

#include <karin/graphics/linear_gradient_pattern.h>

#include <vulkan/vulkan.h>
#include <unordered_map>
#include <array>

namespace karin
{
class VulkanDeviceResources
{
public:
    explicit VulkanDeviceResources(VulkanGraphicsDevice* device, size_t maxFramesInFlight)
        : m_device(device), m_maxFramesInFlight(maxFramesInFlight)
    {
        if (!m_device)
        {
            throw std::runtime_error("VulkanDeviceResources: device is null");
        }

        createSampler();
    }

    ~VulkanDeviceResources() = default;

    VkDescriptorSet gradientPointLutDescriptorSet(const LinearGradientPattern& pattern) const;

private:
    struct LutTexture
    {
        VkImage image = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
        VkImageView imageView = VK_NULL_HANDLE;
        VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
    };

    static constexpr size_t LUT_WIDTH = 256;

    void createSampler();
    std::array<uint8_t, LUT_WIDTH * 4> generateGradientPointLut(
        const std::vector<LinearGradientPattern::GradientPoint>& gradientPoints
    ) const;

    std::unordered_map<size_t, VkDescriptorSet> m_gradientPointLutMap;

    VulkanGraphicsDevice* m_device = nullptr;

    VkSampler m_sampler = VK_NULL_HANDLE;
    uint32_t m_maxFramesInFlight = 2;
};
} // karin

#endif //SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H
