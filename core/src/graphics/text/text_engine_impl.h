#ifndef SRC_GRAPHICS_TEXT_TEXT_ENGINE_IMPL_H
#define SRC_GRAPHICS_TEXT_TEXT_ENGINE_IMPL_H

#include <karin/graphics/text_blob.h>
#include <karin/graphics/text_style.h>
#include <karin/graphics/paragraph_style.h>

#include "font_loader.h"
#include "text_layouter.h"

namespace karin
{
class TextEngineImpl
{
public:
    TextEngineImpl();
    ~TextEngineImpl() = default;

    TextBlob layoutText(
        const std::string& text,
        const TextStyle& textStyle,
        const ParagraphStyle& paragraphStyle,
        const Size& maxSize
    ) const;

private:
    std::unique_ptr<FontLoader> m_fontLoader;
    std::unique_ptr<TextLayouter> m_textLayouter;
};
}


#endif //SRC_GRAPHICS_TEXT_TEXT_ENGINE_IMPL_H