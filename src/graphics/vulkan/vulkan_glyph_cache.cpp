#include "vulkan_glyph_cache.h"

namespace karin
{
VulkanGlyphCache::VulkanGlyphCache(
    VulkanGraphicsDevice* device, VkDescriptorSetLayout atlasDescriptorSetLayout, size_t maxFramesInFlight
)
{
}

VulkanGlyphCache::~VulkanGlyphCache()
{
}

VulkanGlyphCache::GlyphInfo VulkanGlyphCache::getGlyph(std::string character, const Font& font)
{
}

size_t VulkanGlyphCache::glyphKey(std::string character, const Font& font)
{
}
} // karin
