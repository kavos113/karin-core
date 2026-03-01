#ifndef SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H
#define SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H

#include "vulkan_glyph_cache.h"
#include "vma.h"
#include <text/text_layouter.h>

#include <karin/graphics/image.h>
#include <karin/graphics/pattern.h>

#include <vulkan/vulkan.h>
#include <vector>
#include <cstddef>
#include <unordered_map>
#include <array>

namespace karin
{
class VulkanDeviceResources
{
public:
    explicit VulkanDeviceResources(
        size_t maxFramesInFlight
    )
        : m_maxFramesInFlight(maxFramesInFlight)
    {
        createSamplers();
        createDescriptorSetLayouts();
        createDummyTexture();
    }

    ~VulkanDeviceResources() = default;

    Image createImage(const std::vector<std::byte>& data, uint32_t width, uint32_t height);

    void cleanup();

    std::vector<VkDescriptorSet> gradientPointLutDescriptorSet(const GradientPoints& points);
    std::vector<VkDescriptorSet> textureDescriptorSet(Image image);
    std::vector<VkDescriptorSet> dummyTextureDescriptorSet() const;

    VkDescriptorSetLayout geometryDescriptorSetLayout() const
    {
        return m_geometryDescriptorSetLayout;
    }

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
    void createDescriptorSetLayouts();
    void createDummyTexture();
    std::array<uint8_t, LUT_WIDTH * 4> generateGradientPointLut(
        const std::vector<GradientPoints::GradientPoint>& gradientPoints
    ) const;

    std::unordered_map<size_t, Texture> m_gradientPointLutMap;
    std::unordered_map<size_t, Texture> m_textureMap;
    Texture m_dummyTexture; // 1 x 1 white pixel

    VkSampler m_clampSampler = VK_NULL_HANDLE;
    VkSampler m_repeatSampler = VK_NULL_HANDLE;
    VkSampler m_mirrorSampler = VK_NULL_HANDLE;
    uint32_t m_maxFramesInFlight = 2;
    VkDescriptorSetLayout m_geometryDescriptorSetLayout = VK_NULL_HANDLE;
};
} // karin

#endif //SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H