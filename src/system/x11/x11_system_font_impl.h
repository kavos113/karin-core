#ifndef SRC_SYSTEM_X11_X11_SYSTEM_FONT_IMPL_H
#define SRC_SYSTEM_X11_X11_SYSTEM_FONT_IMPL_H

#include <system_font_impl.h>

#include <karin/system/font.h>
#include <vector>
#include <cstddef>

namespace karin
{
class X11SystemFontImpl : SystemFontImpl
{
public:
    X11SystemFontImpl();
    ~X11SystemFontImpl() override;

    std::vector<Font> getSystemFonts() override;
    std::vector<std::byte> getFontData(const Font& font) override;
};
}

#endif //SRC_SYSTEM_X11_X11_SYSTEM_FONT_IMPL_H