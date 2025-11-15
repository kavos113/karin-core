#ifndef SRC_GRAPHICS_TEXT_FONT_LOADER_H
#define SRC_GRAPHICS_TEXT_FONT_LOADER_H

#include <karin/system/system_font.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>

#include "utils/hash.h"

namespace karin
{
class FontLoader
{
public:
    struct FontMetrics
    {
        float width = 0.0f;
        float height = 0.0f;
        float bearingX = 0.0f;
        float bearingY = 0.0f;
        float advanceX = 0.0f;
    };

    FontLoader(SystemFont* systemFont);
    ~FontLoader();

    FT_Face loadFont(const Font& font);
    FontMetrics glyphMetrics(const Font& font, uint32_t size, uint32_t glyphIndex);

private:
    struct Glyph
    {
        Font font;
        uint32_t glyphIndex;
        uint32_t size;

        size_t hash() const
        {
            size_t seed = 0;
            hash_combine(seed, font.hash());
            hash_combine(seed, glyphIndex);
            hash_combine(seed, size);

            return seed;
        }
    };

    SystemFont* m_systemFont = nullptr;

    FT_Library m_library = nullptr;

    std::unordered_map<size_t, FT_Face> m_faces;
    std::unordered_map<size_t, FontMetrics> m_glyphMetricsCache;

    std::vector<std::vector<std::byte>> m_fontDataStorage;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FONT_LOADER_H