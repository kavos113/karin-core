#include <utils/string.h>
#include <gtest/gtest.h>

class StringUtilsTest : public ::testing::Test
{
protected:
    std::string sampleString = "Hello, Karin! 日本語";
    std::wstring sampleWString = L"Hello, Karin! 日本語";
};

TEST_F(StringUtilsTest, WStringToStringConversion)
{
    std::string converted = toString(sampleWString);
    EXPECT_EQ(converted, sampleString);
}

TEST_F(StringUtilsTest, StringToWStringConversion)
{
    std::wstring converted = toWString(sampleString);
    EXPECT_EQ(converted, sampleWString);
}

TEST_F(StringUtilsTest, EmptyStringConversion)
{
    std::string emptyStr = "";
    std::wstring emptyWStr = L"";

    EXPECT_EQ(toString(emptyWStr), emptyStr);
    EXPECT_EQ(toWString(emptyStr), emptyWStr);
}