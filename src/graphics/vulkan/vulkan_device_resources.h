#ifndef SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H
#define SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H

#include "vulkan_glyph_cache.h"
#include "vulkan_graphics_device.h"

#include <karin/graphics/image.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/text_layout.h>

#include <vulkan/vulkan.h>
#include <vulkan/vma.h>
#include <vector>
#include <cstddef>
#include <stdexcept>
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

    std::vector<VulkanGlyphCache::GlyphInfo> textLayout(const TextLayout& layout);

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
    std::unordered_map<size_t, std::vector<VulkanGlyphCache::GlyphInfo>> m_textLayoutCache;

    std::unique_ptr<VulkanGlyphCache> m_glyphCache;

    VulkanGraphicsDevice* m_device = nullptr;

    VkSampler m_clampSampler = VK_NULL_HANDLE;
    VkSampler m_repeatSampler = VK_NULL_HANDLE;
    VkSampler m_mirrorSampler = VK_NULL_HANDLE;
    uint32_t m_maxFramesInFlight = 2;
    VkDescriptorSetLayout m_textureDescriptorSetLayout = VK_NULL_HANDLE;
};
} // karin

#endif //SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H
