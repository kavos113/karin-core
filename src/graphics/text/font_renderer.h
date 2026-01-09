#ifndef SRC_GRAPHICS_TEXT_FONT_RENDERER_H
#define SRC_GRAPHICS_TEXT_FONT_RENDERER_H

#include <memory>

#include "font_renderer_platform.h"

namespace karin
{
class FontRenderer
{
private:
    std::unique_ptr<IPlatformFontRenderer> m_platformRenderer;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FONT_RENDERER_H