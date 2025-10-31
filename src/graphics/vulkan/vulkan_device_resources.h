#ifndef SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H
#define SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H

#include "vulkan_glyph_cache.h"
#include "vulkan_graphics_device.h"
#include "vma.h"
#include <text/font_loader.h>
#include <text/font_layouter.h>

#include <karin/graphics/image.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/text_layout.h>

#include <vulkan/vulkan.h>
#include <vector>
#include <cstddef>
#include <stdexcept>
#include <unordered_map>
#include <array>
#include <memory>

namespace karin
{
class VulkanDeviceResources
{
public:
    explicit VulkanDeviceResources(
        VulkanGraphicsDevice* device,
        std::unique_ptr<FontLoader> fontLoader,
        size_t maxFramesInFlight
    )
        : m_fontLoader(std::move(fontLoader)), m_device(device), m_maxFramesInFlight(maxFramesInFlight)
    {
        if (!m_device)
        {
            throw std::runtime_error("VulkanDeviceResources: device is null");
        }

        createSamplers();
        createDescriptorSetLayouts();
        createDummyTexture();

        m_glyphCache = std::make_unique<VulkanGlyphCache>(m_device, maxFramesInFlight);
        m_fontLayouter = std::make_unique<FontLayouter>(m_fontLoader.get());
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

    VkDescriptorSetLayout atlasDescriptorSetLayout() const
    {
        return m_glyphCache->atlasDescriptorSetLayout();
    }

    struct GlyphPosition
    {
        // position in layout. pixels
        Rectangle position;

        // uv in atlas. 0.0 - 1.0
        Rectangle uv;
    };

    std::vector<GlyphPosition> textLayout(const TextLayout& layout);

    void flushGlyphUploads() const
    {
        m_glyphCache->flushUploadQueue();
    }

    std::vector<VkDescriptorSet> glyphAtlasDescriptorSets() const
    {
        return m_glyphCache->atlasDescriptorSets();
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
    std::unordered_map<size_t, std::vector<GlyphPosition>> m_textLayoutCache;

    std::unique_ptr<VulkanGlyphCache> m_glyphCache;
    std::unique_ptr<FontLoader> m_fontLoader;
    std::unique_ptr<FontLayouter> m_fontLayouter;

    VulkanGraphicsDevice* m_device = nullptr;

    VkSampler m_clampSampler = VK_NULL_HANDLE;
    VkSampler m_repeatSampler = VK_NULL_HANDLE;
    VkSampler m_mirrorSampler = VK_NULL_HANDLE;
    uint32_t m_maxFramesInFlight = 2;
    VkDescriptorSetLayout m_geometryDescriptorSetLayout = VK_NULL_HANDLE;
};
} // karin

#endif //SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H