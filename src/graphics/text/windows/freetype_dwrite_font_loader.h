#ifndef SRC_GRAPHICS_TEXT_WINDOWS_FREETYPE_DWRITE_FONT_LOADER_H
#define SRC_GRAPHICS_TEXT_WINDOWS_FREETYPE_DWRITE_FONT_LOADER_H

#include <font_loader_platform.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <memory>

namespace karin
{
class FreeTypeDWriteFontLoader : public IPlatformFontLoader
{
public:
    FreeTypeDWriteFontLoader();
    ~FreeTypeDWriteFontLoader() override;

    std::unique_ptr<IFontFace> loadFont(const Font& font) override;
    std::unique_ptr<IFontFace> loadFontFromFile(const std::string& filePath) override;
    std::vector<Font> getFontLists() override;

private:
    FT_Library m_library = nullptr;
    std::vector<std::vector<std::byte>> m_fontDataStorage;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_WINDOWS_FREETYPE_DWRITE_FONT_LOADER_H