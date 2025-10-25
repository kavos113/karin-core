#include "font_layouter.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

#include <ranges>
#include <string>
#include <stdexcept>

namespace
{
hb_direction_t toHBDirection(karin::TextFormat::Direction dir)
{
    switch (dir)
    {
    case karin::TextFormat::Direction::LEFT_TO_RIGHT:
        return HB_DIRECTION_LTR;
    case karin::TextFormat::Direction::RIGHT_TO_LEFT:
        return HB_DIRECTION_RTL;
    case karin::TextFormat::Direction::TOP_TO_BOTTOM:
        return HB_DIRECTION_TTB;
    case karin::TextFormat::Direction::BOTTOM_TO_TOP:
        return HB_DIRECTION_BTT;
    default:
        return HB_DIRECTION_LTR;
    }
}
}

namespace karin
{
std::vector<FontLayouter::GlyphPosition> FontLayouter::layout(const TextLayout &layout) const
{
    FT_Face face = m_fontLoader->loadFont(layout.format.font);
    FT_Error error = FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(layout.format.size));
    if (error)
    {
        throw std::runtime_error("failed to set pixel sizes for font");
    }

    std::vector<std::string> lines = std::views::split(layout.text, '\n')
        | std::views::transform(
              [](auto &&rng)
              {
                  return std::string(rng.begin(), rng.end());
              })
        | std::ranges::to<std::vector<std::string>>();

    std::vector<GlyphPosition> glyphs;

    hb_direction_t direction = toHBDirection(layout.format.readingDirection);
    hb_font_t* hbFont = hb_ft_font_create(face, nullptr);

    for (const auto& line : lines)
    {
        hb_buffer_t* hbBuffer = hb_buffer_create();
        hb_buffer_add_utf8(hbBuffer, line.c_str(), -1, 0, -1);

        hb_buffer_set_direction(hbBuffer, direction);
        hb_buffer_set_language(hbBuffer, hb_language_from_string(layout.format.locale.c_str(), -1));

        hb_shape(hbFont, hbBuffer, nullptr, 0);

        unsigned int glyphCount;
        hb_glyph_info_t* glyphInfo = hb_buffer_get_glyph_infos(hbBuffer, &glyphCount);

        for (unsigned int i = 0; i < glyphCount; i++)
        {
            FT_UInt glyphIndex = glyphInfo[i].codepoint;
            GlyphPosition gPos = {
                .position = Rectangle{}, // Position calculation can be added here
                .glyphIndex = glyphIndex,
            };
            glyphs.push_back(gPos);
        }

        hb_buffer_destroy(hbBuffer);
    }

    return glyphs;
}
} // karin