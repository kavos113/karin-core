#include <karin/common/color/color.h>

#include <gtest/gtest.h>

TEST(ColorTest, Create)
{
    karin::Color color(0.5f, 0.5f, 0.5f);
    ASSERT_FLOAT_EQ(color.r, 0.5f);
    ASSERT_FLOAT_EQ(color.g, 0.5f);
    ASSERT_FLOAT_EQ(color.b, 0.5f);
    ASSERT_FLOAT_EQ(color.a, 1.0f);
}

TEST(ColorTest, CreateWithAlpha)
{
    karin::Color color(0.5f, 0.5f, 0.5f, 0.8f);
    ASSERT_FLOAT_EQ(color.r, 0.5f);
    ASSERT_FLOAT_EQ(color.g, 0.5f);
    ASSERT_FLOAT_EQ(color.b, 0.5f);
    ASSERT_FLOAT_EQ(color.a, 0.8f);
}

TEST(ColorTest, CreateFromRGB)
{
    karin::Color color(0xFF0000);
    ASSERT_FLOAT_EQ(color.r, 1.0f);
    ASSERT_FLOAT_EQ(color.g, 0.0f);
    ASSERT_FLOAT_EQ(color.b, 0.0f);
    ASSERT_FLOAT_EQ(color.a, 1.0f);
}

TEST(ColorTest, CreateFromRGBWithAlpha)
{
    karin::Color color(0xFF0000, 0.5f);
    ASSERT_FLOAT_EQ(color.r, 1.0f);
    ASSERT_FLOAT_EQ(color.g, 0.0f);
    ASSERT_FLOAT_EQ(color.b, 0.0f);
    ASSERT_FLOAT_EQ(color.a, 0.5f);
}

TEST(ColorTest, Equality)
{
    karin::Color color1(0.5f, 0.5f, 0.5f);
    karin::Color color2(0.5f, 0.5f, 0.5f);
    karin::Color color3(0.6f, 0.5f, 0.5f);
    ASSERT_EQ(color1, color2);
    ASSERT_NE(color1, color3);
}

TEST(ColorTest, KnownColors)
{
    ASSERT_EQ(
        karin::Color(karin::Color::Red),
        karin::Color(1.0f, 0.0f, 0.0f, 1.0f)
    );
    ASSERT_EQ(
        karin::Color(karin::Color::Green),
        karin::Color(0.0f, 1.0f, 0.0f, 1.0f)
    );
    ASSERT_EQ(
        karin::Color(karin::Color::Blue),
        karin::Color(0.0f, 0.0f, 1.0f, 1.0f)
    );
    ASSERT_EQ(
        karin::Color(karin::Color::White),
        karin::Color(1.0f, 1.0f, 1.0f, 1.0f)
    );
    ASSERT_EQ(
        karin::Color(karin::Color::Black),
        karin::Color(0.0f, 0.0f, 0.0f, 1.0f)
    );
    ASSERT_EQ(
        karin::Color(karin::Color::Yellow),
        karin::Color(1.0f, 1.0f, 0.0f, 1.0f)
    );
    ASSERT_EQ(
        karin::Color(karin::Color::Cyan),
        karin::Color(0.0f, 1.0f, 1.0f, 1.0f)
    );
    ASSERT_EQ(
        karin::Color(karin::Color::Magenta),
        karin::Color(1.0f, 0.0f, 1.0f, 1.0f)
    );
}