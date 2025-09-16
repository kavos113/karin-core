#ifndef SRC_GRAPHICS_VULKAN_VULKAN_GLYPH_CACHE_H
#define SRC_GRAPHICS_VULKAN_VULKAN_GLYPH_CACHE_H

#include "vulkan_graphics_device.h"

#include <karin/common/geometry/rectangle.h>
#include <karin/system/font.h>
#include <text/font_loader.h>

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
        VulkanGraphicsDevice* device,
        FontLoader* fontLoader,
        size_t maxFramesInFlight
    );
    ~VulkanGlyphCache() = default;

    void cleanup();

    struct GlyphInfo
    {
        // coordinates in the atlas. 0.0 - 1.0
        Rectangle uv{};

        // glyph metrics. pixels
        float width = 0.0f;
        float height = 0.0f;
        float bearingX = 0.0f;
        float bearingY = 0.0f;
        float advanceX = 0.0f;
    };

    struct GlyphUploadInfo
    {
        std::vector<std::byte> bitmapData;
        int width = 0;
        int height = 0;

        Point atlasRegion{0.0f, 0.0f};
    };

    GlyphInfo getGlyph(unsigned int glyphIndex, const Font& font, float size);
    void flushUploadQueue();

private:
    static size_t glyphKey(unsigned int glyphInde, const Font& font, float size);

    Point allocateAtlasSpace(int width, int height);

    void createAtlas();
    void createDescriptorSetLayout();
    void createSampler();
    void transitionLayout(VkCommandBuffer commandBuffer, VkImageLayout newLayout);

    static constexpr int ATLAS_WIDTH = 2048;
    static constexpr int ATLAS_HEIGHT = 2048;

    static constexpr float SIZE_FLOAT_ACCURACY = 100.0f;

    FontLoader* m_fontLoader;

    VulkanGraphicsDevice* m_device = nullptr;
    size_t m_maxFramesInFlight = 2;

    std::unordered_map<size_t, GlyphInfo> m_glyphMap;
    std::vector<GlyphUploadInfo> m_uploadQueue;

    VkImage m_atlasImage = VK_NULL_HANDLE;
    VmaAllocation m_atlasImageAllocation = VK_NULL_HANDLE;
    VkImageView m_atlasImageView = VK_NULL_HANDLE;
    VkImageLayout m_atlasImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    std::vector<VkDescriptorSet> m_atlasDescriptorSets; // One per frame in flight
    VkSampler m_atlasSampler = VK_NULL_HANDLE;
    VkDescriptorSetLayout m_atlasDescriptorSetLayout = VK_NULL_HANDLE;

    int m_currentX = 0;
    int m_currentY = 0;
    int m_rowHeight = 0;
};
} // karin

#endif //SRC_GRAPHICS_VULKAN_VULKAN_GLYPH_CACHE_H
