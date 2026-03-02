#ifndef SRC_GRAPHICS_VULKAN_VULKAN_FONT_RENDERER_H
#define SRC_GRAPHICS_VULKAN_VULKAN_FONT_RENDERER_H

#include "font_renderer_impl.h"

#include "vulkan_glyph_cache.h"

namespace karin
{
class VulkanRendererImpl;

class VulkanFontRenderer : public IFontRendererImpl
{
public:
    explicit VulkanFontRenderer(VulkanRendererImpl *renderer, size_t maxFramesInFlight);
    ~VulkanFontRenderer() override;

    void drawText(const TextBlob& text, Point start, Pattern& pattern, const Transform2D& transform) const override;

    void cleanup() const;

    VkDescriptorSetLayout atlasDescriptorSetLayout() const
    {
        return m_glyphCache->atlasDescriptorSetLayout();
    }

    void flushGlyphUploads() const
    {
        m_glyphCache->flushUploadQueue();
    }

    std::vector<VkDescriptorSet> glyphAtlasDescriptorSets() const
    {
        return m_glyphCache->atlasDescriptorSets();
    }

private:
    struct GlyphUploadPosition
    {
        // origin: top-left of the glyph bitmap
        Rectangle position;
        Rectangle atlasUV; // in [0, 1]
    };

    std::unique_ptr<VulkanGlyphCache> m_glyphCache;

    VulkanRendererImpl* m_renderer;
};
} // karin

#endif //SRC_GRAPHICS_VULKAN_VULKAN_FONT_RENDERER_H