#ifndef SRC_GRAPHICS_TEXT_FONT_RENDERER_PLATFORM_H
#define SRC_GRAPHICS_TEXT_FONT_RENDERER_PLATFORM_H

#include "font_face.h"
#include "text_layouter.h"

namespace karin
{
class IPlatformFontRenderer
{
    virtual ~IPlatformFontRenderer() = default;

    virtual void renderText(const std::vector<TextLayouter::GlyphPosition>& glyphs, IFontFace* face, float fontSize) = 0;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FONT_RENDERER_PLATFORM_H