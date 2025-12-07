#ifndef SRC_SYSTEM_SYSTEM_FONT_IMPL_H
#define SRC_SYSTEM_SYSTEM_FONT_IMPL_H

#include <karin/system/font.h>

#include <vector>

namespace karin
{
class ISystemFontImpl
{
public:
    virtual ~ISystemFontImpl() = default;

    virtual std::vector<Font> getSystemFonts() = 0;
    virtual std::vector<std::byte> getFontData(const Font& font) = 0;
};
} // karin

#endif //SRC_SYSTEM_SYSTEM_FONT_IMPL_H