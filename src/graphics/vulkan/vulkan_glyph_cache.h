#ifndef SRC_GRAPHICS_VULKAN_VULKAN_GLYPH_CACHE_H
#define SRC_GRAPHICS_VULKAN_VULKAN_GLYPH_CACHE_H

#include "vulkan_graphics_device.h"

#include <karin/common/geometry/rectangle.h>
#include <karin/system/font.h>

#include <vulkan/vulkan.h>
#include <vulkan/vma.h>
#include <unordered_map>
#include <string>
#include <vector>

namespace karin
{
class VulkanGlyphCache
{
public:
    explicit VulkanGlyphCache(
        VulkanGraphicsDevice* device, VkDescriptorSetLayout atlasDescriptorSetLayout, size_t maxFramesInFlight
    );
    ~VulkanGlyphCache();

    struct GlyphInfo
    {
        // coordinates in the atlas
        Rectangle uv{};

        // glyph metrics
        float width = 0.0f;
        float height = 0.0f;
        float bearingX = 0.0f;
        float bearingY = 0.0f;
        float advanceX = 0.0f;
    };

    GlyphInfo getGlyph(std::string character, const Font& font);

private:
    static size_t glyphKey(std::string character, const Font& font);

    static constexpr int ATLAS_WIDTH = 2048;
    static constexpr int ATLAS_HEIGHT = 2048;

    VulkanGraphicsDevice* m_device = nullptr;
    VkDescriptorSetLayout m_atlasDescriptorSetLayout = VK_NULL_HANDLE;
    size_t m_maxFramesInFlight = 2;

    std::unordered_map<size_t, GlyphInfo> m_glyphMap;

    VkImage m_atlasImage = VK_NULL_HANDLE;
    VmaAllocation m_atlasImageAllocation = VK_NULL_HANDLE;
    VkImageView m_atlasImageView = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> m_atlasDescriptorSets; // One per frame in flight
    VkSampler m_atlasSampler = VK_NULL_HANDLE;
};
} // karin

#endif //SRC_GRAPHICS_VULKAN_VULKAN_GLYPH_CACHE_H
