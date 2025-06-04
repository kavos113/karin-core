#ifndef KARIN_COMMON_RECTANGLE_H
#define KARIN_COMMON_RECTANGLE_H
#include "point.h"
#include "size.h"

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

    bool operator==(const Rectangle& other) const;
};

} // karin

#endif //KARIN_COMMON_RECTANGLE_H
