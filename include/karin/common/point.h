#ifndef KARIN_COMMON_POINT_H
#define KARIN_COMMON_POINT_H

namespace karin
{

struct Point
{
    float x;
    float y;

    bool operator==(const Point& point) const
    {
        return x == point.x && y == point.y;
    }

    Point operator+(const Point& point) const
    {
        return {x + point.x, y + point.y};
    }

    Point operator-(const Point& point) const
    {
        return {x - point.x, y - point.y};
    }
};

} // karin

#endif //KARIN_COMMON_POINT_H
