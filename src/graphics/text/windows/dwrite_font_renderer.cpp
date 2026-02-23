#include "dwrite_font_renderer.h"

#include "dwrite_font_face.h"

namespace karin
{
DwriteFontRenderer::DwriteFontRenderer(Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext)
    : m_deviceContext(std::move(deviceContext))
{
}

void DwriteFontRenderer::renderText(const std::vector<TextLayouter::GlyphPosition>& glyphs, IFontFace* face, float fontSize)
{
    DwriteFontFace *dwriteFace = dynamic_cast<DwriteFontFace*>(face);
    if (!dwriteFace)
    {
        throw std::runtime_error("Invalid font face type for DwriteFontRenderer");
    }

    uint32_t glyphCount = static_cast<uint32_t>(glyphs.size());

    std::vector<UINT16> glyphIndices;
    std::vector<FLOAT> glyphAdvances;
    std::vector<DWRITE_GLYPH_OFFSET> glyphOffsets;
    glyphIndices.reserve(glyphCount);
    glyphAdvances.reserve(glyphCount);
    glyphOffsets.reserve(glyphCount);

    for (const auto& glyph : glyphs)
    {
        glyphIndices.push_back(static_cast<UINT16>(glyph.glyphIndex));
        glyphAdvances.push_back(0.0f);

        DWRITE_GLYPH_OFFSET offset;
        offset.advanceOffset = glyph.position.pos.x;
        offset.ascenderOffset = -glyph.position.pos.y;
        glyphOffsets.push_back(offset);
    }

    DWRITE_GLYPH_RUN glyphRun = {
        .fontFace = dwriteFace->face().Get(),
        .fontEmSize = fontSize,
        .glyphCount = glyphCount,
        .glyphIndices = glyphIndices.data(),
        .glyphAdvances = glyphAdvances.data(),
        .glyphOffsets = glyphOffsets.data(),
        .isSideways = FALSE,
        .bidiLevel = 0
    };

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
    HRESULT hr = m_deviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create solid color brush for text rendering");
    }

    m_deviceContext->DrawGlyphRun(
        D2D1::Point2F(0.0f, 0.0f),
        &glyphRun,
        brush.Get(),
        DWRITE_MEASURING_MODE_NATURAL
    );
}
} // karin