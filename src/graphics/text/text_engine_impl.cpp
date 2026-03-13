#include "text_engine_impl.h"

namespace karin
{
TextEngineImpl::TextEngineImpl()
{
    m_fontLoader = std::make_unique<FontLoader>();
    m_textLayouter = std::make_unique<TextLayouter>();
}

TextBlob TextEngineImpl::layoutText(const TextLayout& layout) const
{
    std::unique_ptr<IFontFace> fontFace = m_fontLoader->loadFont(layout.format.font);

    Size layoutSize{};
    std::vector<GlyphPosition> glyphs = TextLayouter::layout(layout, fontFace.get(), layoutSize);

    TextBlob blob = {
        .glyphs = std::move(glyphs),
        .fontFace = std::move(fontFace),
        .fontEmSize = layout.format.size,
        .format = layout.format,
        .layoutSize = layoutSize,
    };

    return blob;
}
}
