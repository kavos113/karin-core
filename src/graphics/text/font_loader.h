#ifndef SRC_GRAPHICS_TEXT_FONT_LOADER_H
#define SRC_GRAPHICS_TEXT_FONT_LOADER_H

#include <karin/system/font.h>
#include "font_loader_platform.h"

#include <memory>

namespace karin
{
class FontLoader
{
public:

private:
    std::unique_ptr<IPlatformFontLoader> m_platformLoader;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FONT_LOADER_H