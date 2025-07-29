#ifndef KARIN_COMMON_COLOR_COLOR_H
#define KARIN_COMMON_COLOR_COLOR_H
#include <cstdint>
#include <iostream>

namespace karin
{

struct Color
{
    enum Enum
    {
        Red = 0xFF0000,
        Green = 0x00FF00,
        Blue = 0x0000FF,
        White = 0xFFFFFF,
        Black = 0x000000,
        Yellow = 0xFFFF00,
        Cyan = 0x00FFFF,
        Magenta = 0xFF00FF
    };

    float r;
    float g;
    float b;
    float a;

    Color() = default;

    Color(float red, float green, float blue, float alpha = 1.0f)
        : r(red), g(green), b(blue), a(alpha) {}

    explicit Color(uint32_t rgb, float alpha = 1.0f)
    {
        r = static_cast<float>((rgb >> 16) & 0xFF) / 255.0f;
        g = static_cast<float>((rgb >> 8) & 0xFF) / 255.0f;
        b = static_cast<float>(rgb & 0xFF) / 255.0f;
        a = alpha;
    }

    explicit Color(Enum knownColor, float alpha = 1.0f)
        : Color(static_cast<uint32_t>(knownColor), alpha) {}

    uint32_t rgb() const
    {
        return (static_cast<uint32_t>(r * 255) << 16) |
               (static_cast<uint32_t>(g * 255) << 8) |
               (static_cast<uint32_t>(b * 255));
    }

    uint32_t rgba() const
    {
        return (static_cast<uint32_t>(r * 255) << 24) |
               (static_cast<uint32_t>(g * 255) << 16) |
               (static_cast<uint32_t>(b * 255) << 8) |
               static_cast<uint32_t>(a * 255);
    }

    bool operator==(const Color& other) const
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    bool operator< (const Color& other) const
    {
        return rgba() < other.rgba();
    }
};

inline std::ostream& operator<<(std::ostream& os, const Color& color)
{
    return os << "Color(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")";
}

} // karin

#endif //KARIN_COMMON_COLOR_COLOR_H
