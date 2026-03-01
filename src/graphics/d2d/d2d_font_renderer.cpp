#include "d2d_font_renderer.h"

#include "windows/dwrite_font_face.h"

namespace karin
{
D2DFontRenderer::D2DFontRenderer(
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext,
    D2DDeviceResources* deviceResources
)
    : m_deviceContext(std::move(d2dContext)),
      m_deviceResources(deviceResources)
{
}

D2DFontRenderer::~D2DFontRenderer() = default;

void D2DFontRenderer::drawText(const TextBlob& text, Point start, Pattern& pattern, const Transform2D& transform) const
{
    auto dwriteFace = dynamic_cast<DwriteFontFace*>(text.fontFace.get());
    if (!dwriteFace)
    {
        throw std::runtime_error("Unsupported font face type");
    }

    std::vector<UINT16> glyphIndices;
    std::vector<DWRITE_GLYPH_OFFSET> glyphOffsets;
    std::vector<float> glyphAdvances;
    glyphIndices.reserve(text.glyphs.size());
    glyphOffsets.reserve(text.glyphs.size());
    glyphAdvances.reserve(text.glyphs.size());

    for (const auto& glyph : text.glyphs)
    {
        glyphIndices.push_back(static_cast<UINT16>(glyph.glyphIndex));
        glyphOffsets.push_back(DWRITE_GLYPH_OFFSET{ glyph.position.x, -glyph.position.y });
        glyphAdvances.push_back(0.0f); // position is calculate by offset
    }

    DWRITE_GLYPH_RUN glyphRun = {
        .fontFace = dwriteFace->face().Get(),
        .fontEmSize = text.fontEmSize,
        .glyphCount = static_cast<UINT32>(text.glyphs.size()),
        .glyphIndices = glyphIndices.data(),
        .glyphAdvances = glyphAdvances.data(),
        .glyphOffsets = glyphOffsets.data(),
        .isSideways = FALSE,
        .bidiLevel = 0
    };

    m_deviceContext->DrawGlyphRun(
        D2D1::Point2F(start.x, start.y),
        &glyphRun,
        m_deviceResources->brush(pattern).Get(),
        DWRITE_MEASURING_MODE_NATURAL
    );
}
} // karin