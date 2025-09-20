#ifndef SRC_SYSTEM_WINDOWS_WIN_FONT_H
#define SRC_SYSTEM_WINDOWS_WIN_FONT_H

#include <system_font_impl.h>

#include <karin/system/font.h>
#include <wrl/client.h>

#include <vector>

class IDWriteFactory;

namespace karin
{
class WinSystemFontImpl : public SystemFontImpl
{
public:
    WinSystemFontImpl();
    ~WinSystemFontImpl() override;

    std::vector<Font> getSystemFonts() override;
    std::vector<std::byte> getFontData(const Font& font) override;

private:
    Microsoft::WRL::ComPtr<IDWriteFactory> m_factory;
    HMODULE m_dwriteModule = nullptr;
};
}


#endif //SRC_SYSTEM_WINDOWS_WIN_FONT_H
