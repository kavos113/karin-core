#ifndef KARIN_GRAPHICS_TEXT_ENGINE_H
#define KARIN_GRAPHICS_TEXT_ENGINE_H

#include "text_blob.h"
#include "text_style.h"
#include "paragraph_style.h"

#include <memory>

namespace karin
{
class TextEngineImpl;

class TextEngine
{
public:
    TextEngine();
    ~TextEngine();

    TextBlob layoutText(
        const std::string& text,
        const TextStyle& textStyle,
        const ParagraphStyle& paragraphStyle,
        const Size& maxSize
    );

private:
    std::unique_ptr<TextEngineImpl> m_impl;
};
}

#endif //KARIN_GRAPHICS_TEXT_ENGINE_H