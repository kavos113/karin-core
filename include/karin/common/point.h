#ifndef KARIN_COMMON_POINT_H
#define KARIN_COMMON_POINT_H

namespace karin
{

struct Point
{
    float x;
    float y;

    bool operator==(const Point& point) const;
    Point operator+(const Point& point) const;
    Point operator-(const Point& point) const;
};

} // karin

#endif //KARIN_COMMON_POINT_H
