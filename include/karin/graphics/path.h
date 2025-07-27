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
    ~Path();

    void start(Point start) const;
    void lineTo(Point end) const;
    void arcTo(
        Point center,
        float radiusX,
        float radiusY,
        float startAngle,
        float endAngle
    ) const;
    void close() const;

private:
    std::unique_ptr<PathImpl> m_impl;

private:
    friend class GraphicsContext;

    PathImpl* impl() const;
};

} // karin

#endif //KARIN_GRAPHICS_GRAPHICS_PATH_H
