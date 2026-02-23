#ifndef SRC_GRAPHICS_TEXT_FONT_RENDERER_H
#define SRC_GRAPHICS_TEXT_FONT_RENDERER_H

#include <memory>

#include "font_renderer_platform.h"
#include "font_face.h"
#include "text_layouter.h"

namespace karin
{
class FontRenderer
{
public:
    FontRenderer() = default;
    ~FontRenderer() = default;

    void renderText(const std::vector<TextLayouter::GlyphPosition>& glyphs, IFontFace* face);

private:
    std::unique_ptr<IPlatformFontRenderer> m_platformRenderer;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FONT_RENDERER_H