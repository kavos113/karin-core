#ifndef KARIN_SYSTEM_FONT_H
#define KARIN_SYSTEM_FONT_H

#include "font.h"

#include <memory>
#include <vector>
#include <cstddef>

namespace karin
{
class ISystemFontImpl;

class SystemFont
{
public:
    SystemFont();

    std::vector<Font> getSystemFonts() const;
    std::vector<std::byte> getFontData(const Font& font) const;

private:
    std::unique_ptr<ISystemFontImpl> m_impl;
};
}

#endif //KARIN_SYSTEM_FONT_H