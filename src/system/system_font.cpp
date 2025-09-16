#include "platform.h"

#include <karin/system/system_font.h>

namespace karin
{
SystemFont::SystemFont()
    : m_impl(createSystemFontImpl())
{
    if (!m_impl)
    {
        throw std::runtime_error("Failed to create SystemFont implementation for this platform.");
    }
}

std::vector<Font> SystemFont::getSystemFonts() const
{
    return m_impl->getSystemFonts();
}

std::vector<std::byte> SystemFont::getFontData(const Font& font) const
{
    return m_impl->getFontData(font);
}
}