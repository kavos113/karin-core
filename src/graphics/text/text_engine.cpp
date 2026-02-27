#include <karin/graphics/text_engine.h>

#include "text_engine_impl.h"

namespace karin
{
TextEngine::TextEngine()
{
    m_impl = std::make_unique<TextEngineImpl>();
}

TextBlob TextEngine::layoutText(const TextLayout& layout) const
{
    return m_impl->layoutText(layout);
}
}
