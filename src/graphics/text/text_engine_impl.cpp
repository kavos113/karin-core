#include "text_engine_impl.h"

namespace karin
{
TextEngineImpl::TextEngineImpl()
{
    m_fontLoader = std::make_unique<FontLoader>();
    m_textLayouter = std::make_unique<TextLayouter>();
}

TextBlob TextEngineImpl::layoutText(
    const std::string& text,
    const TextStyle& textStyle,
    const ParagraphStyle& paragraphStyle,
    const Size& maxSize
) const
{
    std::unique_ptr<IFontFace> fontFace = m_fontLoader->loadFont(textStyle.font);

    Size layoutSize{};
    std::vector<GlyphPosition> glyphs = TextLayouter::layout(
        fontFace.get(),
        text,
        textStyle,
        paragraphStyle,
        maxSize,
        layoutSize
    );

    return TextBlob{
        .glyphs = std::move(glyphs),
        .fontFace = std::move(fontFace),
        .fontEmSize = textStyle.size,
        .format = textStyle,
        .layoutSize = layoutSize,
    };
}
}
