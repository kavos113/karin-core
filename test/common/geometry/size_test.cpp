#include <karin/common/geometry/size.h>

#include <gtest/gtest.h>

class SizeTest : public testing::Test
{
};

TEST_F(SizeTest, CreateSize)
{
    constexpr karin::Size size{.width = 1.0f, .height = 2.0f};

    ASSERT_EQ(size.width, 1.0f);
    ASSERT_EQ(size.height, 2.0f);
}

TEST_F(SizeTest, Equality)
{
    constexpr karin::Size size1{.width = 1.0f, .height = 2.0f};
    constexpr karin::Size size2{.width = 1.0f, .height = 2.0f};

    ASSERT_EQ(size1, size2);
}