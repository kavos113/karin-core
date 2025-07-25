#ifndef KARIN_GRAPHICS_GRAPHICS_PATH_H
#define KARIN_GRAPHICS_GRAPHICS_PATH_H

#include <memory>
#include <karin/common/geometry/point.h>

namespace karin
{

class PathImpl;

class Path
{
public:
    Path();
    ~Path() = default;

    void start(Point start);
    void lineTo(Point end);
    void arcTo(
        Point center,
        float radiusX,
        float radiusY,
        float startAngle,
        float endAngle
    );
    void close();

private:
    std::unique_ptr<PathImpl> m_impl;
};

} // karin

#endif //KARIN_GRAPHICS_GRAPHICS_PATH_H
