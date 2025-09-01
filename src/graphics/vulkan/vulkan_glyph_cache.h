#ifndef SRC_GRAPHICS_VULKAN_VULKAN_GLYPH_CACHE_H
#define SRC_GRAPHICS_VULKAN_VULKAN_GLYPH_CACHE_H
#include <karin/common/geometry/rectangle.h>
#include <karin/system/font.h>
#include <unordered_map>

namespace karin
{
class VulkanGlyphCache
{
public:
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

private:
    static size_t glyphKey(std::string character, const Font& font);

    std::unordered_map<size_t, GlyphInfo> m_glyphMap;
};
} // karin

#endif //SRC_GRAPHICS_VULKAN_VULKAN_GLYPH_CACHE_H
