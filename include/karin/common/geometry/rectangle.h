#ifndef KARIN_COMMON_RECTANGLE_H
#define KARIN_COMMON_RECTANGLE_H
#include "point.h"
#include "size.h"

#include <iostream>

namespace karin
{

struct Rectangle
{
    Point pos;
    Size size;

    Rectangle() = default;
    Rectangle(const Point& position, const Size& size)
        : pos(position), size(size) {}
    Rectangle(float x, float y, float width, float height)
        : pos{x, y}, size{width, height} {}

    bool operator==(const Rectangle& other) const
    {
        return pos == other.pos && size == other.size;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Rectangle& rect)
{
    return os << "Rect( " << rect.pos << ", " << rect.size << " )";
}

} // karin

#endif //KARIN_COMMON_RECTANGLE_H
