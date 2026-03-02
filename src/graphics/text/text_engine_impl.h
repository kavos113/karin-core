#ifndef SRC_GRAPHICS_TEXT_TEXT_ENGINE_IMPL_H
#define SRC_GRAPHICS_TEXT_TEXT_ENGINE_IMPL_H

#include <karin/graphics/text_layout.h>
#include <karin/graphics/text_blob.h>

#include "font_loader.h"
#include "text_layouter.h"

namespace karin
{
class TextEngineImpl
{
public:
    TextEngineImpl();
    ~TextEngineImpl() = default;

    TextBlob layoutText(const TextLayout& layout) const;

private:
    std::unique_ptr<FontLoader> m_fontLoader;
    std::unique_ptr<TextLayouter> m_textLayouter;
};
}


#endif //SRC_GRAPHICS_TEXT_TEXT_ENGINE_IMPL_H