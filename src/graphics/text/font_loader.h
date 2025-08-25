#ifndef SRC_GRAPHICS_TEXT_FONT_LOADER_H
#define SRC_GRAPHICS_TEXT_FONT_LOADER_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <unordered_map>

namespace karin
{
class FontLoader
{
public:
    FontLoader();
    ~FontLoader();

private:
    FT_Library m_library = nullptr;

    std::unordered_map<std::string, FT_Face> m_faces;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FONT_LOADER_H
