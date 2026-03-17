#include "text_layouter.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <harfbuzz/hb.h>

#include <ranges>
#include <string>
#include <vector>

#include "harfbuzz_provider.h"

namespace
{
using namespace karin;

hb_direction_t toHBDirection(ParagraphStyle::Direction dir)
{
    switch (dir)
    {
    case ParagraphStyle::Direction::LEFT_TO_RIGHT:
        return HB_DIRECTION_LTR;
    case ParagraphStyle::Direction::RIGHT_TO_LEFT:
        return HB_DIRECTION_RTL;
    case ParagraphStyle::Direction::TOP_TO_BOTTOM:
        return HB_DIRECTION_TTB;
    case ParagraphStyle::Direction::BOTTOM_TO_TOP:
        return HB_DIRECTION_BTT;
    default:
        return HB_DIRECTION_LTR;
    }
}

float calculateLineHeight(
    float lineSpacing,
    ParagraphStyle::LineSpacingMode mode,
    float fontSize,
    float metricsHeight
)
{
    switch (mode)
    {
    case ParagraphStyle::LineSpacingMode::PROPORTIONAL:
        return fontSize * lineSpacing;
    case ParagraphStyle::LineSpacingMode::UNIFORM:
        return lineSpacing;
    default:
        return metricsHeight;
    }
}

float calculateBaseLine(
    float baseline,
    ParagraphStyle::LineSpacingMode mode,
    float fontSize,
    float metricsHeight
)
{
    switch (mode)
    {
    case ParagraphStyle::LineSpacingMode::PROPORTIONAL:
        return fontSize * baseline;
    case ParagraphStyle::LineSpacingMode::UNIFORM:
        return baseline;
    default:
        return metricsHeight;
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

// TODO: breakable has two pattern:
// 1. spaces: don't rendered at first of new line
// 2. CJK characters: need to be rendered at first of new line
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
}

namespace karin
{
std::vector<GlyphPosition> TextLayouter::layout(
    const IFontFace* face,
    const std::string& text,
    const TextStyle& textStyle,
    const ParagraphStyle& paragraphStyle,
    const Size& maxSize,
    Size& outContentSize
)
{
    std::vector<std::string> lines = std::views::split(text, '\n')
        | std::views::transform(
            [](auto&& rng)
            {
                return std::string(rng.begin(), rng.end());
            }
        )
        | std::ranges::to<std::vector<std::string>>();

    auto hbProvider = dynamic_cast<const IHarfBuzzProvider*>(face);
    if (!hbProvider)
    {
        throw std::runtime_error("FontFace must implement IHarfBuzzProvider");
    }

    hb_font_t* hbFont = hbProvider->getHbFont();

    FontMetrics fontMetrics = face->getFontMetrics();
    std::vector<GlyphPosition> glyphs;

    float initPenX = 0;
    float scale = textStyle.size / static_cast<float>(fontMetrics.unitsPerEm);
    float lineHeight = calculateLineHeight(
        paragraphStyle.lineSpacing,
        paragraphStyle.lineSpacingMode,
        textStyle.size,
        static_cast<float>(fontMetrics.capHeight) * scale
    );
    float penY = calculateBaseLine(
        paragraphStyle.baseline,
        paragraphStyle.lineSpacingMode,
        textStyle.size,
        static_cast<float>(fontMetrics.capHeight) * scale
    );
    float penX = 0;
    float maxX = 0;

    for (const auto& line : lines)
    {
        hb_buffer_t* hbBuffer = hb_buffer_create();
        hb_buffer_add_utf8(hbBuffer, line.c_str(), -1, 0, -1);
        hb_buffer_set_direction(hbBuffer, toHBDirection(paragraphStyle.readingDirection));
        hb_buffer_set_language(hbBuffer, hb_language_from_string(textStyle.locale.c_str(), -1));

        hb_shape(hbFont, hbBuffer, nullptr, 0);

        uint32_t glyphCount = 0;
        hb_glyph_info_t* glyphInfo = hb_buffer_get_glyph_infos(hbBuffer, &glyphCount);
        hb_glyph_position_t* glyphPos = hb_buffer_get_glyph_positions(hbBuffer, &glyphCount);

        std::vector<GlyphMetrics> metricsList;
        metricsList.reserve(glyphCount);
        for (uint32_t i = 0; i < glyphCount; i++)
        {
            FT_UInt glyphIndex = glyphInfo[i].codepoint;

            auto metrics = face->getGlyphMetrics(glyphIndex);
            metrics.advanceX = metrics.advanceX * scale;
            metrics.bearingX = metrics.bearingX * scale;
            metrics.bearingY = metrics.bearingY * scale;
            metrics.width = metrics.width * scale;
            metrics.height = metrics.height * scale;

            metricsList.push_back(metrics);
        }

        uint32_t lastSpaceIndex = 0;
        for (uint32_t i = 0; i < glyphCount; i++)
        {
            if (isBreakable(getCodepoint(line, glyphInfo[i].cluster)))
            {
                lastSpaceIndex = i;
            }

            Point position = {
                penX + metricsList[i].bearingX,
                penY,
            };

            glyphs.push_back(GlyphPosition{
                .position = position,
                .glyphIndex = metricsList[i].glyphIndex,
            });
            penX += static_cast<float>(glyphPos[i].x_advance) * scale;

            if (maxSize.width > 0 && penX > maxSize.width)
            {
                maxX = maxSize.width;

                if (paragraphStyle.wrapping == ParagraphStyle::Wrapping::NONE)
                {
                    if (paragraphStyle.trimming == ParagraphStyle::Trimming::CHARACTER)
                    {
                        glyphs.pop_back();
                        break;
                    }

                    if (paragraphStyle.trimming == ParagraphStyle::Trimming::WORD && lastSpaceIndex != 0)
                    {
                        for (uint32_t j = i; j > lastSpaceIndex; j--)
                        {
                            glyphs.pop_back();
                        }
                        glyphs.pop_back();
                        break;
                    }
                }
                else if (paragraphStyle.wrapping == ParagraphStyle::Wrapping::CHARACTER)
                {
                    penX = initPenX;
                    penY += lineHeight;
                    glyphs.pop_back();
                    i--;
                }
                else if (paragraphStyle.wrapping == ParagraphStyle::Wrapping::WORD && lastSpaceIndex != 0)
                {
                    penX = initPenX;
                    penY += lineHeight;
                    for (uint32_t j = i; j >= lastSpaceIndex; j--)
                    {
                        glyphs.pop_back();
                    }
                    // TODO: change depend on breakable character type. Now: all rendered at first of new line.
                    i = lastSpaceIndex - 1;
                    lastSpaceIndex = 0;
                }
            }
        }

        hb_buffer_destroy(hbBuffer);

        maxX = std::max(maxX, penX);

        penX = initPenX;
        penY += lineHeight;
    }

    outContentSize = {
        .width = maxX,
        .height = penY,
    };

    return glyphs;
}
} // karin