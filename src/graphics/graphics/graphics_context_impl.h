#ifndef SRC_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_IMPL_H
#include <karin/common/geometry/rectangle.h>
#include <karin/common/color/pattern.h>

namespace karin
{

class IGraphicsContextImpl
{
public:
    virtual ~IGraphicsContextImpl() = default;

    virtual void fillRect(Rectangle rect, Pattern* pattern) = 0;
    virtual void fillEllipse(Point center, float radiusX, float radiusY, Pattern *pattern) = 0;
    virtual void fillRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern *pattern) = 0;

    virtual void drawLine(Point start, Point end, Pattern *pattern, StrokeStyle strokeStyle) = 0;
    virtual void drawRect(Rectangle rect, Pattern *pattern, StrokeStyle strokeStyle) = 0;
    virtual void drawEllipse(Point center, float radiusX, float radiusY, Pattern *pattern, StrokeStyle strokeStyle) = 0;
    virtual void drawRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern *pattern, StrokeStyle strokeStyle) = 0;
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_IMPL_H
