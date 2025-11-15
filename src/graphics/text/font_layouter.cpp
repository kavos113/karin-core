#include "font_layouter.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

#include <ranges>
#include <string>
#include <vector>

namespace
{
using namespace karin;

hb_direction_t toHBDirection(TextFormat::Direction dir)
{
    switch (dir)
    {
    case TextFormat::Direction::LEFT_TO_RIGHT:
        return HB_DIRECTION_LTR;
    case TextFormat::Direction::RIGHT_TO_LEFT:
        return HB_DIRECTION_RTL;
    case TextFormat::Direction::TOP_TO_BOTTOM:
        return HB_DIRECTION_TTB;
    case TextFormat::Direction::BOTTOM_TO_TOP:
        return HB_DIRECTION_BTT;
    default:
        return HB_DIRECTION_LTR;
    }
}

float calculateLineHeight(
    float lineSpacing,
    TextFormat::LineSpacingMode mode,
    float fontSize,
    float metricsHeight
)
{
    switch (mode)
    {
    case TextFormat::LineSpacingMode::PROPORTIONAL:
        return fontSize * lineSpacing;
    case TextFormat::LineSpacingMode::UNIFORM:
        return lineSpacing;
    default:
        return metricsHeight * 64.0f;
    }
}

float calculateBaseLine(
    float baseline,
    TextFormat::LineSpacingMode mode,
    float fontSize,
    float metricsHeight
)
{
    switch (mode)
    {
    case TextFormat::LineSpacingMode::PROPORTIONAL:
        return fontSize * baseline;
    case TextFormat::LineSpacingMode::UNIFORM:
        return baseline;
    default:
        return metricsHeight * 64.0f;
    }
}

uint32_t getCodepoint(const std::string& text, size_t index)
{
    if (index >= text.size())
    {
        return 0xFFFD;
    }

    const auto *str = reinterpret_cast<const unsigned char*>(text.c_str());
    unsigned char c = str[index];
    if (c < 0x80)
    {
        return c;
    }
    else if ((c & 0xE0) == 0xC0) // starts with 110
    {
        if (index + 1 >= text.size()) return 0xFFFD;
        return ((c & 0x1F) << 6) | (str[index + 1] & 0x3F);
    }
    else if ((c & 0xF0) == 0xE0) // starts with 1110
    {
        if (index + 2 >= text.size()) return 0xFFFD;
        return ((c & 0x0F) << 12) | ((str[index + 1] & 0x3F) << 6) | (str[index + 2] & 0x3F);
    }
    else if ((c & 0xF8) == 0xF0) // starts with 11110
    {
        if (index + 3 >= text.size()) return 0xFFFD;
        return ((c & 0x07) << 18) | ((str[index + 1] & 0x3F) << 12) | ((str[index + 2] & 0x3F) << 6) | (str[index + 3] & 0x3F);
    }

    return 0;
}

bool isBreakable(uint32_t codepoint)
{
    if (codepoint <= 0x00FF)
    {
        return std::isspace(static_cast<char>(codepoint)) != 0;
    }
    // TODO: Add more comprehensive Unicode breakable character checks. Now: CJK characters are all breakable.
    else if (
        (codepoint >= 0x3000 && codepoint <= 0x9FFF)
    )
    {
        return true;
    }
    return false;
}

struct Metrics
{
    FontLoader::FontMetrics metrics;
    uint32_t glyphIndex;
};
}

namespace karin
{
std::vector<FontLayouter::GlyphPosition> FontLayouter::layout(const TextLayout &layout, FT_Face face) const
{
    std::vector<std::string> lines = std::views::split(layout.text, '\n')
        | std::views::transform(
            [](auto&& rng)
            {
                return std::string(rng.begin(), rng.end());
            }
        )
        | std::ranges::to<std::vector<std::string>>();

    hb_font_t* hbFont = hb_ft_font_create(face, nullptr);
    std::vector<GlyphPosition> glyphs;

    float initPenX = 0;
    float lineHeight = calculateLineHeight(
        layout.format.lineSpacing,
        layout.format.lineSpacingMode,
        layout.format.size,
        static_cast<float>(face->size->metrics.height)
    );
    float penY = calculateBaseLine(
        layout.format.baseline,
        layout.format.lineSpacingMode,
        layout.format.size,
        static_cast<float>(face->size->metrics.height)
    );
    float penX = 0;

    for (const auto& line : lines)
    {
        hb_buffer_t* hbBuffer = hb_buffer_create();
        hb_buffer_add_utf8(hbBuffer, line.c_str(), -1, 0, -1);
        hb_buffer_set_direction(hbBuffer, toHBDirection(layout.format.readingDirection));
        hb_buffer_set_language(hbBuffer, hb_language_from_string(layout.format.locale.c_str(), -1));

        hb_shape(hbFont, hbBuffer, nullptr, 0);

        uint32_t glyphCount = 0;
        hb_glyph_info_t* glyphInfo = hb_buffer_get_glyph_infos(hbBuffer, &glyphCount);
        hb_glyph_position_t* glyphPos = hb_buffer_get_glyph_positions(hbBuffer, &glyphCount);

        std::vector<Metrics> metricsList;
        metricsList.reserve(glyphCount);
        for (uint32_t i = 0; i < glyphCount; i++)
        {
            FT_UInt glyphIndex = glyphInfo[i].codepoint;
            auto metrics = m_fontLoader->glyphMetrics(layout.format.font, static_cast<uint32_t>(layout.format.size), glyphIndex);
            metricsList.emplace_back(metrics, glyphIndex);
        }

        uint32_t lastSpaceIndex = 0;
        for (uint32_t i = 0; i < glyphCount; i++)
        {
            if (isBreakable(getCodepoint(line, glyphInfo[i].cluster)))
            {
                lastSpaceIndex = i;
            }

            Rectangle position = {
                penX + metricsList[i].metrics.bearingX,
                penY - metricsList[i].metrics.bearingY,
                metricsList[i].metrics.width,
                metricsList[i].metrics.height
            };

            glyphs.push_back(GlyphPosition{
                .position = position,
                .glyphIndex = metricsList[i].glyphIndex,
            });
            penX += static_cast<float>(glyphPos[i].x_advance) / 64.0f;

            if (layout.size.width > 0 && penX > layout.size.width)
            {
                if (layout.format.wrapping == TextFormat::Wrapping::NONE)
                {
                    if (layout.format.trimming == TextFormat::Trimming::CHARACTER)
                    {
                        glyphs.pop_back();
                        break;
                    }

                    if (layout.format.trimming == TextFormat::Trimming::WORD && lastSpaceIndex != 0)
                    {
                        for (uint32_t j = i; j >= lastSpaceIndex; j--)
                        {
                            glyphs.pop_back();
                        }
                        break;
                    }
                }
                else if (layout.format.wrapping == TextFormat::Wrapping::CHARACTER)
                {
                    penX = initPenX;
                    penY += lineHeight;
                    glyphs.pop_back();
                    i--;
                }
                else if (layout.format.wrapping == TextFormat::Wrapping::WORD && lastSpaceIndex != 0)
                {
                    penX = initPenX;
                    penY += lineHeight;
                    for (uint32_t j = i; j >= lastSpaceIndex; j--)
                    {
                        glyphs.pop_back();
                    }
                    i = lastSpaceIndex;
                    lastSpaceIndex = 0;
                }
            }
        }

        hb_buffer_destroy(hbBuffer);

        penX = initPenX;
        penY += lineHeight;
    }

    hb_font_destroy(hbFont);

    return glyphs;
}
} // karin