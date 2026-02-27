#ifndef KARIN_GRAPHICS_TEXT_ENGINE_H
#define KARIN_GRAPHICS_TEXT_ENGINE_H

#include "text_blob.h"
#include "text_layout.h"

#include <memory>

namespace karin
{
class TextEngineImpl;

class TextEngine
{
public:
    TextEngine();
    ~TextEngine() = default;

    TextBlob layoutText(const TextLayout& layout) const;

private:
    std::unique_ptr<TextEngineImpl> m_impl;
};
}

#endif //KARIN_GRAPHICS_TEXT_ENGINE_H