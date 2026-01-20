#ifndef SRC_GRAPHICS_TEXT_FREETYPE_FREETYPE_FONT_FACE_H
#define SRC_GRAPHICS_TEXT_FREETYPE_FREETYPE_FONT_FACE_H

#include <font_face.h>

#include <hb.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace karin
{
class FreetypeFontFace : public IFontFace
{
public:
    FreetypeFontFace(FT_Face face);
    ~FreetypeFontFace() override;

    hb_font_t* getHbFont() override;
    FT_Face face();

private:
    hb_font_t *m_fbFont;
    FT_Face m_face;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FREETYPE_FREETYPE_FONT_FACE_H