#ifndef SRC_GRAPHICS_TEXT_FONT_LOADER_H
#define SRC_GRAPHICS_TEXT_FONT_LOADER_H

#include <karin/system/system_font.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>

namespace karin
{
class FontLoader
{
public:
    FontLoader(SystemFont* systemFont);
    ~FontLoader();

    FT_Face loadFont(const Font& font);

private:
    SystemFont* m_systemFont = nullptr;

    FT_Library m_library = nullptr;

    std::unordered_map<size_t, FT_Face> m_faces;
    std::vector<std::vector<std::byte>> m_fontDataStorage;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FONT_LOADER_H
