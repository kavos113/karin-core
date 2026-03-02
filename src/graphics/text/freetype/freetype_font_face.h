#ifndef SRC_GRAPHICS_TEXT_FREETYPE_FREETYPE_FONT_FACE_H
#define SRC_GRAPHICS_TEXT_FREETYPE_FREETYPE_FONT_FACE_H

#include <harfbuzz_provider.h>
#include <karin/graphics/font_face.h>

#include <hb.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace karin
{
class FreetypeFontFace : public IFontFace, public IHarfBuzzProvider
{
public:
    FreetypeFontFace(FT_Face face);
    ~FreetypeFontFace() override;

    FreetypeFontFace(const FreetypeFontFace&) = delete;
    FreetypeFontFace& operator=(const FreetypeFontFace&) = delete;

    hb_font_t* getHbFont() const override;
    FontMetrics getFontMetrics() const override;
    GlyphMetrics getGlyphMetrics(uint32_t glyphIndex) const override;
    FT_Face face();

private:
    hb_font_t *m_fbFont;
    FT_Face m_face;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FREETYPE_FREETYPE_FONT_FACE_H