#ifndef SRC_GRAPHICS_TEXT_WINDOWS_DWRITE_FONT_LOADER_H
#define SRC_GRAPHICS_TEXT_WINDOWS_DWRITE_FONT_LOADER_H

#include <text/font_loader_platform.h>
#include <karin/graphics/font_face.h>

#include <vector>
#include <memory>

namespace karin
{
class DwriteFontLoader : public IPlatformFontLoader
{
public:
    DwriteFontLoader() = default;
    ~DwriteFontLoader() override;

    std::unique_ptr<IFontFace> loadFont(const Font& font) override;
    std::unique_ptr<IFontFace> loadFontFromFile(const std::string& filePath) override;
    std::vector<Font> getFontLists() override;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_WINDOWS_DWRITE_FONT_LOADER_H