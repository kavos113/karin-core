#ifndef SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H
#define SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H

#include "vulkan_graphics_device.h"

#include <karin/graphics/image.h>
#include <karin/graphics/pattern.h>
#include <text/font_loader.h>

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

        createSamplers();
        createDescriptorSetLayout();
    }

    ~VulkanDeviceResources() = default;

    Image createImage(const std::vector<std::byte>& data, uint32_t width, uint32_t height);

    void cleanup();

    std::vector<VkDescriptorSet> gradientPointLutDescriptorSet(const GradientPoints& points);

    std::vector<VkDescriptorSet> textureDescriptorSet(Image image);
    VkDescriptorSetLayout textureDescriptorSetLayout() const;

private:
    struct Texture
    {
        VkImage image = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
        VkImageView imageView = VK_NULL_HANDLE;
        std::vector<VkDescriptorSet> descriptorSets; // One per frame in flight
    };

    static constexpr size_t LUT_WIDTH = 256;

    void createSamplers();
    void createDescriptorSetLayout();
    std::array<uint8_t, LUT_WIDTH * 4> generateGradientPointLut(
        const std::vector<GradientPoints::GradientPoint>& gradientPoints
    ) const;

    std::unordered_map<size_t, Texture> m_gradientPointLutMap;
    std::unordered_map<size_t, Texture> m_textureMap;

    VulkanGraphicsDevice* m_device = nullptr;

    std::unique_ptr<FontLoader> m_fontLoader;

    VkSampler m_clampSampler = VK_NULL_HANDLE;
    VkSampler m_repeatSampler = VK_NULL_HANDLE;
    VkSampler m_mirrorSampler = VK_NULL_HANDLE;
    uint32_t m_maxFramesInFlight = 2;
    VkDescriptorSetLayout m_textureDescriptorSetLayout = VK_NULL_HANDLE;
};
} // karin

#endif //SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H
