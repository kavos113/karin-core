#ifndef SRC_GRAPHICS_GRAPHICS_PATH_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_PATH_IMPL_H

#include <karin/common/geometry/point.h>

namespace karin
{

class IPathImpl
{
public:
    virtual ~IPathImpl() = default;

    virtual void start(Point start) = 0;
    virtual void lineTo(Point end) = 0;
    virtual void arcTo(
        Point center,
        float radiusX,
        float radiusY,
        float startAngle,
        float endAngle
    ) = 0;
    virtual void close() = 0;
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_PATH_IMPL_H
