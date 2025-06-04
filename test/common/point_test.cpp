#include <karin/common/point.h>

#include <gtest/gtest.h>

class PointTest : public testing::Test
{
protected:
    static constexpr float DEFAULT_X = 3;
    static constexpr float DEFAULT_Y = 4;
};

TEST_F(PointTest, initialize)
{
    constexpr karin::Point p1(DEFAULT_X, DEFAULT_Y);
    constexpr karin::Point p2{.x = DEFAULT_X, .y = DEFAULT_Y};
    const auto [x, y](p1);

    ASSERT_EQ(p1.x, DEFAULT_X);
    ASSERT_EQ(p1.y, DEFAULT_Y);
    ASSERT_EQ(p2.x, DEFAULT_X);
    ASSERT_EQ(p2.y, DEFAULT_Y);
    ASSERT_EQ(x, DEFAULT_X);
    ASSERT_EQ(y, DEFAULT_Y);
}

TEST_F(PointTest, equality)
{
    constexpr karin::Point p1(DEFAULT_X, DEFAULT_Y);
    constexpr karin::Point p2(DEFAULT_X, DEFAULT_Y);
    constexpr karin::Point p3(DEFAULT_X + 1, DEFAULT_Y);
    constexpr karin::Point p4(DEFAULT_X, DEFAULT_Y + 1);

    ASSERT_EQ(p1, p2);
    ASSERT_NE(p1, p3);
    ASSERT_NE(p1, p4);
}

TEST_F(PointTest, addition)
{
    constexpr karin::Point p1(DEFAULT_X, DEFAULT_Y);
    constexpr karin::Point p2(DEFAULT_X + 1, DEFAULT_Y + 1);

    ASSERT_EQ(p1 + p2, karin::Point((DEFAULT_X * 2) + 1, (DEFAULT_Y * 2) + 1));
}

TEST_F(PointTest, subtraction)
{
    constexpr karin::Point p1(DEFAULT_X, DEFAULT_Y);
    constexpr karin::Point p2(DEFAULT_X + 1, DEFAULT_Y + 1);

    ASSERT_EQ(p2 - p1, karin::Point(1, 1));
}