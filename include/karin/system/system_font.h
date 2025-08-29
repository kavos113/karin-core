#ifndef KARIN_SYSTEM_FONT_H
#define KARIN_SYSTEM_FONT_H

#include "font.h"

#include <memory>
#include <vector>
#include <cstddef>

namespace karin
{
class SystemFontImpl;

class SystemFont
{
public:
    SystemFont();

    std::vector<Font> getSystemFonts();
    std::vector<std::byte> getFontData(const Font& font);

private:
    std::unique_ptr<SystemFontImpl> m_impl;
};
}

#endif //KARIN_SYSTEM_FONT_H
