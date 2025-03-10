#include "geometry/Point.h"

namespace karin
{

bool Point::operator==(const Point& point) const
{
    return x == point.x && y == point.y;
}

Point Point::operator+(const Point& point) const
{
    return Point{.x = x + point.x, .y = y + point.y};
}

Point Point::operator-(const Point& point) const
{
    return Point{.x = x - point.x, .y = y - point.y};
}

}  // namespace karin