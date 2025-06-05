#include <karin/common/geometry/rectangle.h>

#include <gtest/gtest.h>

#include <karin/common/geometry/point.h>
#include <karin/common/geometry/size.h>

class RectangleTest : public testing::Test
{
protected:
    static constexpr float X = 1.0f;
    static constexpr float Y = 2.0f;
    static constexpr float WIDTH = 3.0f;
    static constexpr float HEIGHT = 4.0f;
};

TEST_F(RectangleTest, CreateRectangle)
{
    const karin::Rectangle rect(
        karin::Point{X, Y},
        karin::Size{WIDTH, HEIGHT}
    );

    ASSERT_EQ(rect.pos.x, X);
    ASSERT_EQ(rect.pos.y, Y);
    ASSERT_EQ(rect.size.width, WIDTH);
    ASSERT_EQ(rect.size.height, HEIGHT);

    const karin::Rectangle rect3(X, Y, WIDTH, HEIGHT);
    ASSERT_EQ(rect3.pos.x, X);
    ASSERT_EQ(rect3.pos.y, Y);
    ASSERT_EQ(rect3.size.width, WIDTH);
    ASSERT_EQ(rect3.size.height, HEIGHT);
}

TEST_F(RectangleTest, Equality)
{
    const karin::Rectangle rect1(
        karin::Point{X, Y},
        karin::Size{WIDTH, HEIGHT}
    );
    const karin::Rectangle rect2(
        karin::Point{X, Y},
        karin::Size{WIDTH, HEIGHT}
    );
    const karin::Rectangle rect3(
        karin::Point{X + 1, Y},
        karin::Size{WIDTH, HEIGHT}
    );
    const karin::Rectangle rect4(
        karin::Point{X, Y + 1},
        karin::Size{WIDTH, HEIGHT}
    );

    ASSERT_EQ(rect1, rect2);
    ASSERT_NE(rect1, rect3);
    ASSERT_NE(rect1, rect4);
}