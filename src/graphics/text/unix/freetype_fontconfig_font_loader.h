#ifndef SRC_GRAPHICS_TEXT_UNIX_FREETYPE_FONTCONFIG_FONT_LOADER_H
#define SRC_GRAPHICS_TEXT_UNIX_FREETYPE_FONTCONFIG_FONT_LOADER_H

#include <text/font_loader_platform.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace karin
{
class FreeTypeFontConfigFontLoader : public IPlatformFontLoader
{
public:
    FreeTypeFontConfigFontLoader();
    ~FreeTypeFontConfigFontLoader() override;

    std::unique_ptr<IFontFace> loadFont(const Font& font) override;
    std::unique_ptr<IFontFace> loadFontFromFile(const std::string& filePath) override;
    std::vector<Font> getFontLists() override;

private:
    FT_Library m_ftLibrary = nullptr;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_UNIX_FREETYPE_FONTCONFIG_FONT_LOADER_H