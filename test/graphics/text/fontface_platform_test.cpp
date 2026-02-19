#include <gtest/gtest.h>

#include <text/font_loader.h>
#include <text/font_face.h>
#include <text/windows/dwrite_font_loader.h>
#include <text/windows/freetype_dwrite_font_loader.h>
#include <text/windows/dwrite_font_face.h>
#include <text/freetype/freetype_font_face.h>

using namespace karin;

class FontFacePlatformTest : public testing::Test
{
protected:
    DwriteFontLoader loader;
    FreeTypeDWriteFontLoader ftLoader;
};

TEST_F(FontFacePlatformTest, FontMetricsIsSame)
{
    auto fonts = loader.getFontLists();
    ASSERT_FALSE(fonts.empty());

    for (const auto& font : fonts)
    {
        SCOPED_TRACE("Testing font: " + font.family);

        auto dwriteFace = loader.loadFont(font);
        auto ftFace = ftLoader.loadFont(font);

        ASSERT_NE(dwriteFace, nullptr);
        ASSERT_NE(ftFace, nullptr);

        auto dwriteMetrics = dwriteFace->getFontMetrics();
        auto ftMetrics = ftFace->getFontMetrics();

        EXPECT_EQ(dwriteMetrics.unitsPerEm, ftMetrics.unitsPerEm);
        EXPECT_EQ(dwriteMetrics.ascender, ftMetrics.ascender);
        EXPECT_EQ(dwriteMetrics.descender, ftMetrics.descender);
        EXPECT_EQ(dwriteMetrics.lineGap, ftMetrics.lineGap);
        EXPECT_EQ(dwriteMetrics.capHeight, ftMetrics.capHeight);
        EXPECT_EQ(dwriteMetrics.xHeight, ftMetrics.xHeight);
        EXPECT_EQ(dwriteMetrics.underlinePosition, ftMetrics.underlinePosition);
        EXPECT_EQ(dwriteMetrics.underlineThickness, ftMetrics.underlineThickness);
        EXPECT_EQ(dwriteMetrics.strikethroughPosition, ftMetrics.strikethroughPosition);
        EXPECT_EQ(dwriteMetrics.strikethroughThickness, ftMetrics.strikethroughThickness);
    }
}