#ifndef SRC_SYSTEM_WINDOWS_WIN_FONT_H
#define SRC_SYSTEM_WINDOWS_WIN_FONT_H

#include <system_font_impl.h>

#include <karin/system/font.h>
#include <wrl/client.h>
#include <dwrite.h>

#include <vector>

namespace karin
{
class WinSystemFontImpl : public SystemFontImpl
{
public:
    WinSystemFontImpl();
    ~WinSystemFontImpl() override = default;

    std::vector<Font> getSystemFonts() override;
    std::vector<std::byte> getFontData(const Font& font) override;

private:
    Microsoft::WRL::ComPtr<IDWriteFactory> m_factory;
};
}


#endif //SRC_SYSTEM_WINDOWS_WIN_FONT_H
